#ifndef SYSTEMRENDERER_H
#define SYSTEMRENDERER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "fixedv2d.h"
#include "celestialtype.h"

class SystemRenderer : public QOpenGLWidget
{
    Q_OBJECT

public slots:
    void animate();

public:
    SystemRenderer(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override; //mouse backend crap
    void mouseReleaseEvent(QMouseEvent *event) override; //mouse backend crap
    void mouseDoubleClickEvent(QMouseEvent *event) override; //mouse backend crap
    void mouseMoveEvent(QMouseEvent *event) override; //mouse backend crap
    void wheelEvent(QWheelEvent* event) override; //mouse backend crap

private slots:
    void singleClickHelper(void); //mouse backend crap

private:
    // mouse activity callbacks (to flatten out the interface to not be ugly)
    void singleClick(QPoint location);
    void rightClick(QPoint location);
    void doubleClick(QPoint location);
    void clickDrag(QPoint delta);
    void scrollUp(void);
    void scrollDown(void);

    // renderer functions
    void render_planet_recurse(CelestialType *cel);
    QPointF position_to_screen_coordinates(FixedV2D pos);

    bool mouse_pressed;
    int elapsed;
    QTimer clickTimer;

    // for handling click drag
    FixedV2D position;
    //TODO: bracket zooming
    long currentZoom = 40; //zoom factor (exponent of 2)
    QPoint mousedrag_position; // mouse backend crap
    QPoint singleclick_position;
    QPoint center; // center point of screen in pixels

    // render
    QPainter painter;
    //QPen planet; TODO: not necessarily any value in having this one?
    QPen orbit;
};

#endif // SYSTEMRENDERER_H
