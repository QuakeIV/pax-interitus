#include "systemrenderer.h"
#include <QApplication>

// this file handles identifying mouse events on the system renderer and invoking the correct callbacks, IE:
//  - single clicks
//  - double clicks
//  - right clicks
//  - click drags
//  - scrolling

extern QApplication *qapp;

void SystemRenderer::mousePressEvent(QMouseEvent *event)
{
    // manually fixing click position detection
    // TODO: this is either mouse click detection or rendering that is screwing up, either way this is not a good fix
    event->pos().rx() -= 2;
    event->pos().ry() -= 2;
    if (event->buttons() == Qt::LeftButton)
    {
        singleclick_position = mousedrag_position = event->pos(); //grab position for clickdrag stuff and also determining if singleclick should fire (singleclick looks at distance travelled)
        mouse_pressed = true;
        if (clickTimer.isActive())
        {
            clickTimer.stop(); // we just double clicked, stop the timer that will fire single click
            clickTimer.disconnect();
        }
        else
            clickTimer.start(qapp->doubleClickInterval()/2); // going with half of doubleclick interval to fire singleclick, and counting from mousedown
    }

    if (event->buttons() == Qt::RightButton)
    {
        rightClick(event->pos());
    }
}

void SystemRenderer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mouse_pressed = false;
    // we grab mouse drag position when we depress lmb, so let it then inhibit clicks if the delta from starting position is too great
    // manhattan distance is weird but actually works okay for this because it kindof represents the number of times the mouse has moved away a pixel
    if (event->button() == Qt::LeftButton && (singleclick_position - event->pos()).manhattanLength() < 20)
    {
        if (clickTimer.isActive())
        {
            // if timer has not elapsed, we are still in single click territory, so connect up the single click handler
            // this is done here to ignore long clicks, so that you can drag the interface around without clicking stuff
            QObject::connect(&clickTimer, &QTimer::timeout, this, &SystemRenderer::singleClickHelper);
        }
    }

    //TODO: mouseup on rightclick might be fun to have act on right click menus as a expedient (since rightclick fires on mousedown), see if there is a good way to do that
}

void SystemRenderer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        clickTimer.disconnect(); //disconnect singleclick helper since there was a doubleclick
        clickTimer.stop();
        doubleClick(singleclick_position); //TODO: grabbing mousedown position, but this is in effect for the second click. improve?
    }
}

void SystemRenderer::mouseMoveEvent(QMouseEvent *event)
{
    // manually fixing click position detection
    // TODO: this is either mouse click detection or rendering that is screwing up, either way this is not a good fix
    event->pos().rx() -= 2;
    event->pos().ry() -= 2;
    if (event->buttons() == Qt::LeftButton)
    {
        // for now just dont drag if someone is using the yardstick (ctrl-click and mouse move)
        // TODO: probably to be expanded
        //TODO: this isnt just a bitfield as advertised, the values are some jank ass shit
        if (qapp->keyboardModifiers() != Qt::ControlModifier) //== Qt::NoModifier)
            clickDrag(event->pos() - mousedrag_position);
        mousedrag_position = event->pos();
    }
}

void SystemRenderer::wheelEvent(QWheelEvent* event)
{
    // for now only allow zooming if we arent doing something else
    // TODO: maybe this is needless
    if (event->buttons() == 0)
    {
        // angle delta in eights of degree, convert to degrees then divide by 15
        // TODO: most mice are supposed to work in 15 degree increments, so this should account for single scroll events, come back here if that is ever an issue
        int scroll = (event->angleDelta().y() >> 3)/15;

        // TODO: ponder a scrollup/scrolldown handler that takes a number of scrolls, that sounds dumb though this is probably more intuitive to handle
        if (scroll > 0)
            for (int i = 0; i < scroll; i++)
                scrollUp();
        else
            for (int i = 0; i > scroll; i--)
                scrollDown();
    }
}

void SystemRenderer::singleClickHelper(void)
{
    singleClick(singleclick_position); //aha fire single click
    clickTimer.disconnect(); //disconnect singleclick helper since there was a doubleclick
}
