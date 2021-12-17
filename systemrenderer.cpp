#include "systemrenderer.h"

SystemRenderer::SystemRenderer(QWidget *parent) :
    QOpenGLWidget(parent)
{
    elapsed = 0;
    setAutoFillBackground(false);
}

void SystemRenderer::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::white, 12, Qt::DashDotLine, Qt::RoundCap));
    painter.drawLine(0,0,100,100);

    painter.end();
}

void SystemRenderer::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}
