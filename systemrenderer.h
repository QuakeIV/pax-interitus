#ifndef SYSTEMRENDERER_H
#define SYSTEMRENDERER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

class SystemRenderer : public QOpenGLWidget
{
    Q_OBJECT

public slots:
    void animate();

public:
    SystemRenderer(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void singleClickHelper(void);

private:
    bool mouse_pressed;
    int elapsed;
    QTimer clickTimer;
    QMouseEvent *lastSingleClick;

    // current positions in pixel reference frame (to be translated to universe coordinates for purposes of deciding how to render said universe)
    QPoint current_position;
    QPoint previous_position;
};

#endif // SYSTEMRENDERER_H
