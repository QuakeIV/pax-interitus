#include "systemrenderer.h"
#include "fixedv2d.h"
#include "universe.h"

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

    orbit = QPen(Qt::green, 1, Qt::SolidLine, Qt::SquareCap);
}

void SystemRenderer::paintEvent(QPaintEvent *event)
{
    static CelestialType sol = CelestialType(695700000000, 1988500000000000);
    sol.color = QColor(226, 223, 24);

    static CelestialType mercury = CelestialType(2439700000, 330110000, 57909050000000, &sol);

    // calculate middle point so rendering is centered
    center = QPoint(frameGeometry().width(), frameGeometry().height()) / 2;

    painter.begin(this);

    // TODO: ponder more native painting in the future?
    painter.beginNativePainting();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);



    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    painter.endNativePainting();

    render_planet_recurse(&sol);
    painter.end();
}

QPointF SystemRenderer::position_to_screen_coordinates(FixedV2D pos)
{
    FixedV2D a = pos - position;

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
    //render orbits first (if it exists)
    if (cel->trajectory.parent)
    {
        painter.setPen(orbit);
        long pts = cel->trajectory.racetrack_points;
        QPointF start = position_to_screen_coordinates(cel->trajectory.rel_racetrack[pts-1]);

        for (int i = 0; i < pts; i++)
        {
            QPointF next = position_to_screen_coordinates(cel->trajectory.rel_racetrack[i]);
            painter.drawLine(start, next);
            start = next;
        }
    }

    foreach(CelestialType *child, cel->children)
        render_planet_recurse(child);

    // render planet body
    float rad = cel->radius >> currentZoom;
    if (rad < 5.0f)
        rad = 5.0f;
    painter.setPen(cel->color);
    painter.setBrush(cel->color);
    QPointF pos = position_to_screen_coordinates(cel->trajectory.position);
    painter.drawEllipse(pos, rad, rad);
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
    printf("singleclick\n");
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
    position -= d;
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
