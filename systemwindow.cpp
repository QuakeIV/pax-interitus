#include "systemwindow.h"
#include "ui_systemwindow.h"

SystemWindow::SystemWindow(SolarSystemType *system, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SystemWindow)
{
    ui->setupUi(this);

    SystemRenderer *r = this->findChild<SystemRenderer*>("systemview");
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, r, &SystemRenderer::animate);
    // TODO: fiddle with frame rate, configurable frame rate?
    timer->start(50);

    setWindowTitle(system->name + " System View");
}

SystemWindow::~SystemWindow()
{
    delete ui;
}
