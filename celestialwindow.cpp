#include "celestialwindow.h"
#include "ui_celestialwindow.h"

CelestialWindow::CelestialWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CelestialWindow)
{
    ui->setupUi(this);
}

CelestialWindow::~CelestialWindow()
{
    delete ui;
}