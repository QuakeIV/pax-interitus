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

private slots:
    void singleClickHelper(void);

private:
    bool mouse_pressed;
    int elapsed;
    QTimer clickTimer;
    QMouseEvent *lastSingleClick;
};

#endif // SYSTEMRENDERER_H
