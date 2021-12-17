#ifndef SYSTEMRENDERER_H
#define SYSTEMRENDERER_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QPainter>

class SystemRenderer : public QOpenGLWidget
{
    Q_OBJECT

public slots:
    void animate();

public:
    SystemRenderer(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int elapsed;
};

#endif // SYSTEMRENDERER_H
