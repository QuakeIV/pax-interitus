#ifndef PYSYSTEMRENDERER_H
#define PYSYSTEMRENDERER_H

#include <Python.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "fixedv2d.h"
#include "universe/celestial.h"
#include <QMenu>

class Spacecraft;
class PySystemRenderer;

// Python Type
typedef struct
{
    PyObject_HEAD
    PySystemRenderer *renderer;
    PyObject *singleClickCallback;
    PyObject *rightClickCallback;
    PyObject *doubleClickCallback;
} PySystemRendererObject;

extern PyTypeObject PySystemRendererType;

// Qt type
class PySystemRenderer : public QOpenGLWidget
{
    Q_OBJECT

public:
    PySystemRenderer(QWidget *parent = nullptr);

    // python object (to access python callbacks)
    PySystemRendererObject *py_obj = nullptr;

    // you can focus on positions of things and follow them
    FixedV2D *focus;
    SolarSystem *focus_system;
    //this offset is relative to the 'focus' point
    FixedV2D offset;
    // TODO: maybe better to finalize camera position on top of the frame, storing that, then doing rendering?

    // mouse activity
    void singleClick(QPoint location);
    void rightClick(QPoint location);
    void doubleClick(QPoint location);
    void clickDrag(QPoint delta);
    void scrollUp(void);
    void scrollDown(void);

protected:
    void paintEvent(QPaintEvent *event) override;

    // mouse backend
    void mousePressEvent(QMouseEvent *event) override; //mouse backend crap
    void mouseReleaseEvent(QMouseEvent *event) override; //mouse backend crap
    void mouseDoubleClickEvent(QMouseEvent *event) override; //mouse backend crap
    void mouseMoveEvent(QMouseEvent *event) override; //mouse backend crap
    void wheelEvent(QWheelEvent* event) override; //mouse backend crap

private slots:
    void singleClickHelper(void); //mouse backend crap
    void animate(void); //render timer callback

private:
    // renderer functions
    void render_scale(void); //TODO: is this needed in light of yardstick?
    void render_planet_body_recurse(Celestial *cel);
    void render_planet_trajectory_recurse(Celestial *cel);
    void render_yardstick(void);
    void render_fleets(void);
    QPointF position_to_screen_coordinates(FixedV2D pos);

    // recursively search planets to see if a click landed on them, return found planet if any, otherwise return null
    Celestial *planet_click_recurse(Celestial *cel, QPointF p);
    // recursively search fleets to see if a click landed on them
    // (runs after planet click, that way if you click a big pile of crap you get the planet preferentially)
    Spacecraft *spacecraft_click(QPointF p);

    //TODO: bracket zooming
    int64_t currentZoom = 40; //zoom factor (exponent of 2)
    QPoint center; // center point of screen in pixels

    // render
    QPainter painter;
    //QPen planet; TODO: not necessarily any value in having this one?
    QPen orbit;

    // mouse backend
    bool mouse_pressed;
    QTimer clickTimer;
    QPoint mousedrag_position;
    QPoint singleclick_position;
};

#endif // PYSYSTEMRENDERER_H
