#ifndef SYSTEMRENDERER_H
#define SYSTEMRENDERER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

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

    bool mouse_pressed;
    int elapsed;
    QTimer clickTimer;

    // for handling click drag
    QPoint current_position;
    QPoint mousedrag_position; // mouse backend crap
    QPoint singleclick_position;

    // render
    QPainter painter;
    QPen test;
};

#endif // SYSTEMRENDERER_H
