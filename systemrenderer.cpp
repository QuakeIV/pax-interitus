#include "systemrenderer.h"
#include "fixedv2d.h"

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
    test = QPen(Qt::white, 12, Qt::DashDotLine, Qt::RoundCap);
}

void SystemRenderer::paintEvent(QPaintEvent *event)
{
    // calculate middle point so rendering is centered
    QPoint center = QPoint(frameGeometry().width(), frameGeometry().height()) / 2;

    painter.begin(this);
    painter.setPen(test);
    QPoint p1 = QPoint(0,0);
    QPoint p2 = QPoint(100,100);

    // TODO: ponder more native painting in the future?
    painter.beginNativePainting();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    painter.endNativePainting();

    painter.drawLine(p1 + current_position + center, p2 + current_position + center);
    painter.end();
}

void SystemRenderer::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
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
    current_position += delta;
}

void SystemRenderer::scrollUp(void)
{
    printf("scrollup\n");
}

void SystemRenderer::scrollDown(void)
{
    printf("scrolldown\n");
}
