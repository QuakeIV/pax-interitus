#include <Python.h>
#include <structmember.h>
#include "pysystemrenderer.h"
#include "fixedv2d.h"
#include "universe.h"
#include <QApplication>
#include "solarsystemtype.h"
#include <QMenu>
#include "utilities.h"
#include <QLayout>
#include "pywrappers.h"
#include <QSizePolicy>

// Python Type
static PyObject *set_focus(PySystemRendererObject *self, PyObject *args)
{
    PyObject *obj = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "O", &obj))
        Py_RETURN_NONE;

    if (PyObject_IsInstance(obj, (PyObject *)&PyCelestialType))
    {
        PyCelestialObject *cel = (PyCelestialObject *)obj;
        self->renderer->focus = &cel->ref->trajectory;
        self->renderer->offset.x = 0;
        self->renderer->offset.y = 0;
    }
    else if (PyObject_IsInstance(obj, (PyObject *)&PySpacecraftType))
    {
        PySpacecraftObject *s = (PySpacecraftObject *)obj;
        self->renderer->focus = s->ref->trajectory;
        self->renderer->offset.x = 0;
        self->renderer->offset.y = 0;
    }

    // TODO: other things?
    // TODO: add else case+exception?

    Py_RETURN_NONE;
}
static PyMethodDef methods[] = {
    {"set_focus", (PyCFunction)set_focus, METH_VARARGS,
     "Re-focuses the system renderer."
    },
    {NULL}  /* terminate */
};
static PyMemberDef members[] = {
    // for now, singleclick is basically only for refocusing, so we wont call back into python for it
    //{"single_click_callback", T_OBJECT, offsetof(PySystemRendererObject, singleClickCallback), 0,
    // "If set to a callable, is called whenever the system renderer registers a left click.  Effected game objects passed."},
    {"right_click_callback", T_OBJECT, offsetof(PySystemRendererObject, rightClickCallback), 0,
     "If set to a callable, is called whenever the system renderer registers a right click.  Effected game objects passed"},
    {"double_click_callback", T_OBJECT, offsetof(PySystemRendererObject, doubleClickCallback), 0,
     "If set to a callable, is called whenever the system renderer registers a right click.  Effected game objects passed"},
    {NULL}  /* terminate */
};
static void type_dealloc(PySystemRendererObject *self)
{
    Py_XDECREF(self->singleClickCallback);
    Py_XDECREF(self->rightClickCallback);
    Py_XDECREF(self->doubleClickCallback);
    delete self->renderer;
    Py_TYPE(self)->tp_free((PyObject *) self);
}
static PyObject *type_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    uint64_t ptr = 0;
    // Parse arguments
    if(!PyArg_ParseTuple(args, "K", &ptr))
        return NULL;

    PySystemRendererObject *self;
    self = (PySystemRendererObject *) type->tp_alloc(type, 0);
    if (self != NULL)
    {
        self->singleClickCallback = NULL;
        self->rightClickCallback  = NULL;
        self->doubleClickCallback = NULL;

        QLayout *layout = (QLayout *)ptr;
        PySystemRenderer *renderer = new PySystemRenderer(layout->parentWidget());
        layout->addWidget(renderer);
        renderer->py_obj = self;
        self->renderer = renderer;
    }
    return (PyObject *)self;
}
PyTypeObject PySystemRendererType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "paxpython.SystemRenderer",
    .tp_basicsize = sizeof(PySystemRendererObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)type_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc = PyDoc_STR("PaxPython Solar System Renderer"),
    .tp_methods = methods,
    .tp_members = members,
    //.tp_init = (initproc)Custom_init,
    .tp_new = type_new,
};



// Qt Type
PySystemRenderer::PySystemRenderer(QWidget *parent):
  QOpenGLWidget(parent)
{
    //TODO: configurable frame rate, ideally track to that during runtime as well
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PySystemRenderer::animate);
    timer->start(1000.0/60.0); //circa 60 fps? TODO: maybe better frame time mechanism? probably not particularly vital

    clickTimer.setSingleShot(true);

    // make sure we dont get squashed by other widgets
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSizePolicy(policy);

    setAutoFillBackground(false); //TODO: does this do anything useful?

    // enable anti aliasing
    QSurfaceFormat format;
    format.setSamples(16); // multisample sample number
    setFormat(format);
    //test = QPen(Qt::white, 12, Qt::DashDotLine, Qt::RoundCap);

    // TODO: this is temporary, this is not long-term the suitable place to set initial focus i think
    focus_system = systems[0]; //TODO: default to zeroeth system for now (later track home system? maybe last viewed system?)
    focus = &focus_system->root.trajectory;

    orbit = QPen(Qt::green);//, 1, Qt::SolidLine, Qt::SquareCap);
}

void PySystemRenderer::paintEvent(QPaintEvent *event)
{
    universe_lock.lock_shared(); // dont render until we are sure global data isnt being messed with
    // TODO: rendering as a whole needs a lock on universe state, not each renderer instance serially, this could be decongested a lot via redesign
    // maybe a 'readonly' lock and you just check if its set before you go
    // this should probably stay for now though, because it seems to fix the weird drawing jitter that was seen periodically

    // calculate middle point so rendering is centered
    // TODO: only do this on resize? may wind up not working all that well unclear
    center = QPoint(frameGeometry().width(), frameGeometry().height()) / 2;

    painter.begin(this);

    // TODO: ponder more native painting in the future?
    painter.beginNativePainting();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    painter.endNativePainting();

    render_planet_trajectory_recurse(&focus_system->root);
    render_planet_body_recurse(&focus_system->root);

    render_scale();

    render_yardstick();

    render_fleets();

    painter.end();

    universe_lock.unlock_shared();
}
void PySystemRenderer::animate()
{
    update();
}

// TODO: render circles first or something this has an unpleasant degree of repetition
void PySystemRenderer::render_fleets(void)
{
    // copy the list
    QList<Spacecraft*> list = focus_system->spacecraft;

    // sort by y axis to promote displaying text in the right order
    // this did not help readability at all and seemed to impact it negatively but may as well leave this here since i came back to it twice
//    std::sort(list.begin(), list.end(),
//         [](FleetType *a, FleetType *b) { return a->trajectory.position.y < b->trajectory.position.y; });

    // this only exists to track how much vertical room the text needs, so add some padding here
    static float font_size = painter.fontInfo().pixelSize() + 2;

    painter.setPen(Qt::yellow);
    while (list.length())
    {
        // TODO: probably useful for at least guarding rendering of some stuff
//        if (fleet->trajectory.orbital_radius >> currentZoom < 10)
//            continue;
        //TODO: draw trail line?

        Spacecraft *sc = list.takeLast();
        QPointF pos = position_to_screen_coordinates(sc->trajectory->position);
        QPointF offset = QPointF(5,-5); //offset for text display
        //TODO: de-hardcode radius
        painter.drawEllipse(pos, 5.0, 5.0);

        // name
        painter.drawText(pos + offset, sc->name);

        // find colocated fleets
        int64_t i = list.length() - 1;
        while (i >= 0)
        {
            // TODO: would be nice to pre-calculate fleet screen coordinates for the frame
            QPointF copos = position_to_screen_coordinates(list[i]->trajectory->position);
            QPointF d = copos - pos;

            // TODO: needs work to eliminate vertical jitter, shouldnt exist at long range zoom
            // TODO: dont hard code horizontal space?
            if ((abs(d.y()) < font_size) && (abs(d.x()) < 50.0))
            {
                Spacecraft *co_sc = list.takeAt(i);

                offset.ry() -= font_size;

                painter.drawText(pos + offset, co_sc->name);

                // draw circle
                //TODO: de-hardcode radius
                painter.drawEllipse(copos, 5.0, 5.0);
            }

            i--;
        }
    }
}

void PySystemRenderer::render_yardstick(void)
{
    if (!mouse_pressed)
        return;

    if (QApplication::keyboardModifiers() != Qt::ControlModifier)
        return;

    // TODO: just making all of this white for now but not necessarily forever (maybe a milder gray so its not as harsh)
    painter.setPen(Qt::white);

    // draw the line
    painter.drawLine(singleclick_position, mousedrag_position);

    //draw distance
    QPointF d = mousedrag_position - singleclick_position;
    double dist = DISTANCE_FIXED_TO_M((double)sqrt(d.x()*d.x() + d.y()*d.y()) * (double)(1l<<currentZoom));
    painter.drawText(mousedrag_position + QPointF(0,-2), get_distance_str(dist));
}

void PySystemRenderer::render_scale()
{
    //draw scale marker
    static const QPointF scale_from = QPointF(20,30);
    static const QPointF scale_text = QPointF(20,28);
    static const QPointF scale_to   = QPointF(120,30);
    double dist = DISTANCE_FIXED_TO_M((100ul << currentZoom)); //assuming 100px width for now, this needs to stay in sync with the above

    painter.setPen(orbit);
    painter.drawLine(scale_from, scale_to);
    painter.drawText(scale_text, get_distance_str(dist));
}

QPointF PySystemRenderer::position_to_screen_coordinates(FixedV2D pos)
{
    FixedV2D a = pos - (offset + focus->position);

    // TODO: this is capturing remainders so that the anti aliased lines drawn look much nicer, look into optimizing (mainly the float division sucks)
    int64_t mask = (1l << currentZoom) - 1l;
    float factor = 1l<<currentZoom;
    int64_t rem_xi = a.x&mask;
    float rem_xf = (float)rem_xi / factor;
    int64_t rem_yi = a.y&mask;
    float rem_yf = (float)rem_yi / factor;

    a.x >>= currentZoom;
    a.y >>= currentZoom;

    return QPointF((float)a.x + rem_xf, (float)a.y + rem_yf) + center;
}

void PySystemRenderer::render_planet_body_recurse(Celestial *cel)
{
    foreach(Celestial *child, cel->children)
        render_planet_body_recurse(child);

    // render planet body (but only if more center point is more than 5 pixels from parent)
    // if hypothetically both are still minimum size, this would amount to half-overlapping circles
    // if there is no parent body, draw it then as well (sun case)
    if ((cel->trajectory.orbital_radius >> currentZoom) >= 5 || !cel->trajectory.parent)
    {
        float rad = cel->radius >> currentZoom;
        if (rad < 5.0f)
            rad = 5.0f;
        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(cel->color);
        QPointF pos = position_to_screen_coordinates(cel->trajectory.position);
        painter.drawEllipse(pos, rad, rad);
    }
}

void PySystemRenderer::render_planet_trajectory_recurse(Celestial *cel)
{
    // only render orbit if it exists (if parent is set) and if it would be more than 10 pixels wide (otherwise dont bother drawing it its too small)
    if (cel->trajectory.parent && (cel->trajectory.orbital_radius >> currentZoom) >= 10)
    {
        painter.setPen(orbit);
        int64_t pts = cel->trajectory.racetrack_points;
        FixedV2D parent_pos = cel->trajectory.parent->trajectory.position;
        QPointF start = position_to_screen_coordinates(cel->trajectory.rel_racetrack[pts-1] + parent_pos);

        for (int i = 0; i < pts; i++)
        {
            QPointF next = position_to_screen_coordinates(cel->trajectory.rel_racetrack[i] + parent_pos);
            painter.drawLine(start, next);
            start = next;
        }
    }

    foreach(Celestial *child, cel->children)
        render_planet_trajectory_recurse(child);
}

void PySystemRenderer::singleClick(QPoint location)
{
    // TODO: might be nice to keep a root celestial pointer on hand to reduce derefences? possibly over factoring
    Celestial *cel = planet_click_recurse(&focus_system->root, QPointF(location));
    if (cel)
    {
        focus = &cel->trajectory;
        offset.x = 0;
        offset.y = 0;
        return;
    }

    // renaming to task group, probably, its functionally likely to be more fitting
    Spacecraft *spacecraft = spacecraft_click(location);
    if (spacecraft)
    {
        focus = spacecraft->trajectory;
        offset.x = 0;
        offset.y = 0;
        return;
    }

    //TODO: search for other focusable objects (missiles perhaps i guess?)

    // if the click misses, focus onto star and incorporate position of last focused object into offset
    offset += focus->position;
    focus = &focus_system->root.trajectory;
}

// TODO: work out why this has weird timing constraints, there is no double right click so who cares
void PySystemRenderer::rightClick(QPoint location)
{
    // if we dont have a py_obj, dont try to do callback stuff with an object that does not exist
    if (!py_obj)
        return;

    // obtain GIL to safely mess with python objects
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    if (!PyCallable_Check(py_obj->rightClickCallback))
    {
        // no function to call, release GIL
        PyGILState_Release(gstate);
        return;
    }

    PyObject *celestial_list = PyList_New(0);
    foreach (Celestial *c, focus_system->celestials)
    {
        //TODO: it would be nice to calculate display radii and coordinates at render time and then re-use them here, rather than re-calculating
        //TODO: this should be functionalized as its identical to other code
        //TODO: all this rendering crap should probably migrate into a new layer of objects to keep the backend properly divorced from the rendering
        // maybe this frontend layer actually keeps references to the celestials that arent even aware of its existence
        QPointF l = position_to_screen_coordinates(c->trajectory.position);

        float x = l.x() - location.x();
        float y = l.y() - location.y();
        float d = sqrt(x*x + y*y);
        // be a bit generous with click detection for the min-size planets (so 6.0 radius instead of 5.0)
        // TODO: maybe at some point planet display radius will be configurable, definitely call a function for this one
        // TODO: configurable click radius margins?
        if (d < 10 || d < (c->radius >> currentZoom)) // right click margins are more generous
        {
            PyCelestialObject *new_cel = pyobjectize_celestial(c);
            PyList_Append(celestial_list, (PyObject *)new_cel);
            Py_DECREF(new_cel); // decref to undo incref from the newly created object, now that list is holding onto it
        }
    }

    PyObject *spacecraft_list = PyList_New(0);
    foreach (Spacecraft *s, focus_system->spacecraft)
    {
        //TODO: would be nice to pre-compute fleet screen position every frame
        // can track screen corners in fixedv2d form and then calculate which things are displayed, and only update those coords? maybe needless complexity
        QPointF l = position_to_screen_coordinates(s->trajectory->position);

        float x = l.x() - location.x();
        float y = l.y() - location.y();
        float d = sqrt(x*x + y*y);
        // TODO: maybe at some point fleet display radius will be configurable, definitely call a function for this one
        // TODO: configurable click radius margins?
        // TODO: spacecraft will most likely have their own radius eventually
        if (d < 10.0)  // right click margins are more generous
        {
            PySpacecraftObject *new_sc = (PySpacecraftObject *)PyObject_Call((PyObject *)&PySpacecraftType,PyTuple_New(0),NULL);
            new_sc->ref = s;
            PyList_Append(spacecraft_list, (PyObject *)new_sc);
            Py_DECREF(new_sc); // decref to undo incref from the newly created object, now that list is holding onto it
        }
    }

    //QMenu *m = new QMenu("derp", this);
    //m->setAttribute(Qt::WA_DeleteOnClose); //this was tested with the below code to garbage collect the menu
//    connect(m, &QMenu::destroyed,
//            this, [m]() { qDebug() << "deleted" << (qintptr)m; });

//    foreach(Celestial *c, cels)
//    {
//        QMenu *submenu = m->addMenu(c->name);
//        submenu->addAction("Focus", [this, c]()
//        {
//            this->offset.x = 0;
//            this->offset.y = 0;
//            this->focus = &c->trajectory;
//        });

//        submenu->addAction("Info",  [this, c]()
//        {
//            CelestialWindow *w = new CelestialWindow(c,this); // all new windows should root on the main window, not self, so they persist as one might expect
//            w->setAttribute(Qt::WA_DeleteOnClose);
//            w->move(qapp->activeWindow()->mapToGlobal(QPoint(40,20)));
//            w->show();
////            connect(w, &CelestialWindow::destroyed,
////                    this, [w]() { qDebug() << "deleted" << (qintptr)w; });
//        });
//    }

    //if (cels.length() && spacecraft.length())
    //    m->addSeparator();

    //foreach(Spacecraft *s, spacecraft)
    //{
    //    QMenu *submenu = m->addMenu(s->name);
    //    submenu->addAction("Focus", [this, s]()
    //    {
    //        this->offset.x = 0;
    //        this->offset.y = 0;
    //        this->focus = s->trajectory;
    //    });
    //}
    //m->popup(mapToGlobal(location));

    if (!PyObject_CallFunctionObjArgs(py_obj->rightClickCallback, celestial_list, spacecraft_list, NULL))
    {
        PyErr_Print();
        printf("This may have happened because a SystemRenderer had an argument mismatch with its right-click handler.\n");
    }
    //PyObject *args_tuple = PyTuple_Pack(1, celestial_list);
    //PyObject_CallObject(py_obj->rightClickCallback, args_tuple);

    // release GIL
    PyGILState_Release(gstate);
}

void PySystemRenderer::doubleClick(QPoint location)
{
    printf("doubleclick\n");
}

void PySystemRenderer::clickDrag(QPoint delta)
{
    FixedV2D d;
    d.x = ((int64_t)delta.x()) << currentZoom;
    d.y = ((int64_t)delta.y()) << currentZoom;
    offset -= d;
}

void PySystemRenderer::scrollUp(void)
{
    if (currentZoom > 1)
        currentZoom--;
}

void PySystemRenderer::scrollDown(void)
{
    if (currentZoom < 60) //63) //just peg at 60 for now rather than some more theoretical limit
        currentZoom++;
}

Spacecraft * PySystemRenderer::spacecraft_click(QPointF p)
{
    // TODO: track 'current solar system', check that things fleets instead
    foreach (Spacecraft *s, focus_system->spacecraft)
    {
        //TODO: would be nice to pre-compute fleet screen position every frame
        QPointF l = position_to_screen_coordinates(s->trajectory->position);

        float x = l.x() - p.x();
        float y = l.y() - p.y();
        float d = sqrt(x*x + y*y);
        // TODO: maybe at some point fleet display radius will be configurable, definitely call a function for this one
        if (d < 6.0)
            return s;
    }
    return NULL;
}

// return true if a click landed on a planet (this can be used for any type of click)
Celestial * PySystemRenderer::planet_click_recurse(Celestial *cel, QPointF p)
{
    //TODO: it would be nice to calculate display radii and coordinates at render time and then re-use them here, rather than re-calculating
    QPointF l = position_to_screen_coordinates(cel->trajectory.position);

    float x = l.x() - p.x();
    float y = l.y() - p.y();
    float d = sqrt(x*x + y*y);
    // be a bit generous with click detection for the min-size planets (so 6.0 radius instead of 5.0)
    // TODO: maybe at some point planet display radius will be configurable, definitely call a function for this one
    if (d < 6.0 || d < (cel->radius >> currentZoom))
        return cel;

    foreach (Celestial *child, cel->children)
    {
        // only allow clicks on planets that are actually rendering
        // (this is to match the condition in render_planet_recurse that culls planet rendering)
        if ((child->trajectory.orbital_radius >> currentZoom) >= 5)
        {
            //recurse into children
            Celestial *f = planet_click_recurse(child, p);
            if (f)
                return f;
        }
    }
    return NULL;
}
