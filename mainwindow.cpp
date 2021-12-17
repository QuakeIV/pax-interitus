#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SystemRenderer *openGL = this->findChild<SystemRenderer*>("systemview");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, openGL, &SystemRenderer::animate);
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_systemlist_doubleClicked(const QModelIndex &index)
{
    printf("%d\n", index.row());
}

