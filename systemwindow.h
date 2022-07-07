#ifndef SYSTEMWINDOW_H
#define SYSTEMWINDOW_H

#include <QMainWindow>
#include "solarsystemtype.h"
#include "ui_systemwindow.h"

namespace Ui {
class SystemWindow;
}

class SystemWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemWindow(SolarSystemType *system, QWidget *parent):
        QMainWindow(parent),
        ui(new Ui::SystemWindow)
    {
        ui->setupUi(this);

        SystemRenderer *r = this->findChild<SystemRenderer*>("systemview");
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, r, &SystemRenderer::animate);
        // TODO: globally configurable frame rate?
        timer->start(50);

        setWindowTitle(system->name + " System View");
    }

    ~SystemWindow()
    {
        delete ui;
    }

private:
    Ui::SystemWindow *ui;
};

#endif // SYSTEMWINDOW_H
