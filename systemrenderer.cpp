#include "systemrenderer.h"
#include "fixedv2d.h"
#include "universe.h"
#include <QApplication>

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
    focus = &sol->trajectory;

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

    render_planet_recurse(sol);

    render_scale();

    render_yardstick();

    painter.end();
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

    QPointF d = mousedrag_position - singleclick_position;
    double dist = (double)sqrt(d.x()*d.x() + d.y()*d.y()) * (double)(1l<<currentZoom);

    //draw distance
    static const QString si_scale[] = {"m", "", "k","M","G","T"};
    int i;
    for (i = 0; i < (sizeof(si_scale)/sizeof(si_scale[0]) - 1) && dist > 10000.0; i++)
        dist /= 1000;

    painter.drawText(mousedrag_position + QPointF(0,-2), QString("%1%2m").arg(QString::number(dist, 'f', 1), si_scale[i]));
}

void SystemRenderer::render_scale()
{
    //draw scale marker
    static const QPointF scale_from = QPointF(20,30);
    static const QPointF scale_text = QPointF(20,28);
    static const QPointF scale_to   = QPointF(120,30);
    static const QString si_scale[] = {"m", "", "k","M","G","T"};
    double dist = (100l << currentZoom); //assuming 100px width for now, this needs to stay in sync with the above
    int i;
    for (i = 0; i < (sizeof(si_scale)/sizeof(si_scale[0]) - 1) && dist > 10000.0; i++)
        dist /= 1000;

    painter.setPen(orbit);
    painter.drawLine(scale_from, scale_to);
    painter.drawText(scale_text, QString("%1%2m").arg(QString::number(dist, 'f', 1), si_scale[i]));
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
    CelestialType *cel = planet_click_recurse(sol, QPointF(location));
    if (cel)
    {
        focus = &cel->trajectory;
        offset.x = 0;
        offset.y = 0;
        return;
    }

    //TODO: search for fleets and other focusable objects (missiles perhaps i guess?)

    // if the click misses, focus onto star and incorporate position of last focused object into offset
    offset += focus->position;
    focus = &sol->trajectory;
}

void SystemRenderer::rightClick(QPoint location)
{
    printf("rightclick\n");
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
    if (currentZoom > 0)
        currentZoom--;
}

void SystemRenderer::scrollDown(void)
{
    if (currentZoom < 63)
        currentZoom++;
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
