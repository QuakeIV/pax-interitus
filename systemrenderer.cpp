#include "systemrenderer.h"
#include <QApplication>

SystemRenderer::SystemRenderer(QWidget *parent) :
    QOpenGLWidget(parent)
{
    elapsed = 0;
    clickTimer.setSingleShot(true);
    setAutoFillBackground(false); //TODO: does this do anything useful
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

extern QApplication *qapp;

void SystemRenderer::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        mouse_pressed = true;
        if (clickTimer.isActive())
            clickTimer.stop(); // we just doubleclicked, stop the timer that will fire single click
        else
            clickTimer.start(qapp->doubleClickInterval()/2);
        lastSingleClick = event;
    }

    if (event->buttons() == Qt::RightButton)
    {
        printf("rightclick\n");
    }
}

void SystemRenderer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouse_pressed = false;
        if (clickTimer.isActive())
        {
            lastSingleClick = event;
            // if timer has not elapsed, we are still in single click territory, so connect up the single click handler
            // this is done here to ignore long clicks, so that you can drag the interface around without clicking stuff
            QObject::connect(&clickTimer, &QTimer::timeout, this, &SystemRenderer::singleClickHelper);
        }
    }
}

void SystemRenderer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        clickTimer.disconnect(); //disconnect singleclick helper since there was a doubleclick
        clickTimer.stop();
        printf("doubleclick\n");
    }
}

void SystemRenderer::singleClickHelper(void)
{
    printf("singleclick\n");
    clickTimer.disconnect(); //disconnect singleclick helper since there was a doubleclick
}
