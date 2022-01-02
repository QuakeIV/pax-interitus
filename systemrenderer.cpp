#include "systemrenderer.h"
#include "fixedv2d.h"
#include "universe.h"
#include <QApplication>
#include "solarsystemtype.h"
#include <QMenu>
#include "celestialwindow.h"
#include "utilities.h"

extern QApplication *qapp;

SystemRenderer::SystemRenderer(QWidget *parent) :
  QOpenGLWidget(parent)
{
    elapsed = 0;
    clickTimer.setSingleShot(true);
    setAutoFillBackground(false); //TODO: does this do anything useful

    // enable anti aliasing
    QSurfaceFormat format;
    format.setSamples(16); // multisample sample number
    setFormat(format);
    //test = QPen(Qt::white, 12, Qt::DashDotLine, Qt::RoundCap);

    // TODO: this is temporary, this is not long-term the suitable place to set initial focus i think
    focus = &sol.root.trajectory;

    orbit = QPen(Qt::green);//, 1, Qt::SolidLine, Qt::SquareCap);
}

void SystemRenderer::paintEvent(QPaintEvent *event)
{
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

    render_planet_recurse(&sol.root);

    render_scale();

    render_yardstick();

    render_fleets();

    painter.end();
}

// TODO: render circles first or something this has an unpleasant degree of repetition
void SystemRenderer::render_fleets(void)
{
    // copy the list
    QList<FleetType*> list = sol.fleets;

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

        FleetType *fleet = list.takeLast();
        QPointF pos = position_to_screen_coordinates(fleet->trajectory.position);
        QPointF offset = QPointF(5,-5); //offset for text display
        //TODO: de-hardcode radius
        painter.drawEllipse(pos, 5.0, 5.0);

        // name
        painter.drawText(pos + offset, fleet->name);

        // find colocated fleets
        long i = list.length() - 1;
        while (i >= 0)
        {
            // TODO: would be nice to pre-calculate fleet screen coordinates for the frame
            QPointF copos = position_to_screen_coordinates(list[i]->trajectory.position);
            QPointF d = copos - pos;

            // TODO: dont hard code horizontal space?
            if ((abs(d.y()) < font_size) && (abs(d.x()) < 50.0))
            {
                FleetType *co_fleet = list.takeAt(i);

                offset.ry() -= font_size;

                painter.drawText(pos + offset, co_fleet->name);

                // draw circle
                //TODO: de-hardcode radius
                painter.drawEllipse(copos, 5.0, 5.0);
            }

            i--;
        }
    }
}

void SystemRenderer::render_yardstick(void)
{
    if (!mouse_pressed)
        return;

    if (qapp->keyboardModifiers() != Qt::ControlModifier)
        return;

    // TODO: just making all of this white for now but not necessarily forever (maybe a milder gray so its not as harsh)
    painter.setPen(Qt::white);

    // draw the line
    painter.drawLine(singleclick_position, mousedrag_position);

    //draw distance
    QPointF d = mousedrag_position - singleclick_position;
    double dist = (double)sqrt(d.x()*d.x() + d.y()*d.y()) * (double)(1l<<currentZoom);
    painter.drawText(mousedrag_position + QPointF(0,-2), distance_to_str(dist));
}

void SystemRenderer::render_scale()
{
    //draw scale marker
    static const QPointF scale_from = QPointF(20,30);
    static const QPointF scale_text = QPointF(20,28);
    static const QPointF scale_to   = QPointF(120,30);
    double dist = (100ul << currentZoom); //assuming 100px width for now, this needs to stay in sync with the above

    painter.setPen(orbit);
    painter.drawLine(scale_from, scale_to);
    painter.drawText(scale_text, distance_to_str(dist));
}

QPointF SystemRenderer::position_to_screen_coordinates(FixedV2D pos)
{
    FixedV2D a = pos - (offset + focus->position);

    // TODO: this is capturing remainders so that the anti aliased lines drawn look much nicer, look into optimizing (mainly the float division sucks)
    long mask = (1l << currentZoom) - 1l;
    float factor = 1l<<currentZoom;
    long rem_xi = a.x&mask;
    float rem_xf = (float)rem_xi / factor;
    long rem_yi = a.y&mask;
    float rem_yf = (float)rem_yi / factor;

    a.x >>= currentZoom;
    a.y >>= currentZoom;

    return QPointF((float)a.x + rem_xf, (float)a.y + rem_yf) + center;
}

void SystemRenderer::render_planet_recurse(CelestialType *cel)
{
    // render orbits first (if it exists)
    // only render orbit if it exists (if parent is set) and if it would be more than 10 pixels wide (otherwise dont bother drawing it its too small)
    if (cel->trajectory.parent && (cel->trajectory.orbital_radius >> currentZoom) >= 10)
    {
        painter.setPen(orbit);
        long pts = cel->trajectory.racetrack_points;
        FixedV2D parent_pos = cel->trajectory.parent->trajectory.position;
        QPointF start = position_to_screen_coordinates(cel->trajectory.rel_racetrack[pts-1] + parent_pos);

        for (int i = 0; i < pts; i++)
        {
            QPointF next = position_to_screen_coordinates(cel->trajectory.rel_racetrack[i] + parent_pos);
            painter.drawLine(start, next);
            start = next;
        }
    }

    foreach(CelestialType *child, cel->children)
        render_planet_recurse(child);

    // render planet body (but only if more center point is more than 5 pixels from parent)
    // if hypothetically both are still minimum size, this would amount to half-overlapping circles
    // if there is no parent body, then draw it then as well (sun case)
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

void SystemRenderer::animate()
{
    int delta = qobject_cast<QTimer*>(sender())->interval() % 1000;
    elapsed += delta;

    //TODO: this is kindof messy to do this in gui land...
    universe_update(delta);

    update();
}

void SystemRenderer::singleClick(QPoint location)
{
    //TODO: optimally this would grab the current focused solar system rather than just going to the 'sol' variable, this is a placeholder design
    CelestialType *cel = planet_click_recurse(&sol.root, QPointF(location));
    if (cel)
    {
        focus = &cel->trajectory;
        offset.x = 0;
        offset.y = 0;
        return;
    }

    FleetType *fleet = fleet_click(location);
    if (fleet)
    {
        focus = &fleet->trajectory;
        offset.x = 0;
        offset.y = 0;
        return;
    }

    //TODO: search for other focusable objects (missiles perhaps i guess?)

    // if the click misses, focus onto star and incorporate position of last focused object into offset
    offset += focus->position;
    focus = &sol.root.trajectory;
}

void SystemRenderer::rightClick(QPoint location)
{
    // TODO: track current selected solar system instead of hardcoding sol
    QList<CelestialType*> cels;
    foreach (CelestialType* c, sol.celestials)
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
            cels.append(c);
    }

    // TODO: track current selected solar system instead of hardcoding sol
    QList<FleetType*> fleets;
    foreach (FleetType *f, sol.fleets)
    {
        //TODO: would be nice to pre-compute fleet screen position every frame
        // can track screen corners in fixedv2d form and then calculate which things are displayed, and only update those coords? maybe needless complexity
        QPointF l = position_to_screen_coordinates(f->trajectory.position);

        float x = l.x() - location.x();
        float y = l.y() - location.y();
        float d = sqrt(x*x + y*y);
        // TODO: maybe at some point fleet display radius will be configurable, definitely call a function for this one
        // TODO: configurable click radius margins?
        if (d < 10.0)  // right click margins are more generous
            fleets.append(f);
    }

    // TODO: mess with scrolling to make it work better
    QMenu *m = new QMenu("derp", this);
    m->setAttribute(Qt::WA_DeleteOnClose); //this was tested with the below code to garbage collect the menu
//    connect(m, &QMenu::destroyed,
//            this, [m]() { qDebug() << "deleted" << (qintptr)m; });

    foreach(CelestialType *c, cels)
    {
        QMenu *submenu = m->addMenu(c->name);
        submenu->addAction("Focus", [this, c]()
        {
            this->offset.x = 0;
            this->offset.y = 0;
            this->focus = &c->trajectory;
        });

        submenu->addAction("Info",  [this, c]()
        {
            CelestialWindow *w = new CelestialWindow(c,this);
            w->setAttribute(Qt::WA_DeleteOnClose);
            w->move(qapp->activeWindow()->mapToGlobal(QPoint(40,20)));
            w->show();
//            connect(w, &CelestialWindow::destroyed,
//                    this, [w]() { qDebug() << "deleted" << (qintptr)w; });
        });
    }

    if (cels.length() && fleets.length())
        m->addSeparator();

    foreach(FleetType *f, fleets)
    {
        QMenu *submenu = m->addMenu(f->name);
        submenu->addAction("Focus", [this, f]()
        {
            this->offset.x = 0;
            this->offset.y = 0;
            this->focus = &f->trajectory;
        });
    }
    m->popup(mapToGlobal(location));
}

void SystemRenderer::doubleClick(QPoint location)
{
    printf("doubleclick\n");
}

void SystemRenderer::clickDrag(QPoint delta)
{
    FixedV2D d;
    d.x = ((long)delta.x()) << currentZoom;
    d.y = ((long)delta.y()) << currentZoom;
    offset -= d;
}

void SystemRenderer::scrollUp(void)
{
    // TODO: actual min is 0, tweaked to 10 for now so you arent zooming in to mm when ships dont draw which is weird looking
    if (currentZoom > 10)
        currentZoom--;
}

void SystemRenderer::scrollDown(void)
{
    if (currentZoom < 60) //63) //just peg at 60 for now rather than some more theoretical limit
        currentZoom++;
}

FleetType * SystemRenderer::fleet_click(QPointF p)
{
    // TODO: track 'current solar system', check that things fleets instead
    foreach (FleetType *fleet, sol.fleets)
    {
        //TODO: would be nice to pre-compute fleet screen position every frame
        QPointF l = position_to_screen_coordinates(fleet->trajectory.position);

        float x = l.x() - p.x();
        float y = l.y() - p.y();
        float d = sqrt(x*x + y*y);
        // TODO: maybe at some point fleet display radius will be configurable, definitely call a function for this one
        if (d < 6.0)
            return fleet;
    }
    return NULL;
}

// return true if a click landed on a planet (this can be used for any type of click)
CelestialType * SystemRenderer::planet_click_recurse(CelestialType *cel, QPointF p)
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

    foreach (CelestialType *child, cel->children)
    {
        // only allow clicks on planets that are actually rendering
        // (this is to match the condition in render_planet_recurse that culls planet rendering)
        if ((child->trajectory.orbital_radius >> currentZoom) >= 5)
        {
            //recurse into children
            CelestialType *f = planet_click_recurse(child, p);
            if (f)
                return f;
        }
    }
    return NULL;
}
