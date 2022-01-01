#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "universe.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openGL = this->findChild<SystemRenderer*>("systemview");
    pause_button = this->findChild<QPushButton*>("pausebutton");
    display_warp = this->findChild<QPushButton*>("displaywarp");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, openGL, &SystemRenderer::animate);
    // TODO: fiddle with frame rate
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
    //TODO: would be nice to save the vertical splitter spacings and restore when re-opening
}


void MainWindow::on_systemlist_doubleClicked(const QModelIndex &index)
{
    printf("%d\n", index.row());
}


void MainWindow::on_pausebutton_clicked(bool checked)
{
    if (checked)
    {
        universe_paused = true;
        pause_button->setText("PAUSED");
    }
    else
    {
        universe_paused = false;
        pause_button->setText("PAUSE");
    }
}


void MainWindow::on_minuswarp_clicked()
{
    if (universe_time_warp > universe_min_warp)
        universe_time_warp--;
    update_warp_display();
}


void MainWindow::on_pluswarp_clicked()
{
    if (universe_time_warp < universe_max_warp)
        universe_time_warp++;
    update_warp_display();
}


void MainWindow::on_warp1_clicked()
{
    universe_time_warp = 0;
    update_warp_display();
}


void MainWindow::on_warp1k_clicked()
{
    universe_time_warp = 10;
    update_warp_display();
}


void MainWindow::on_warp1M_clicked()
{
    universe_time_warp = 20;
    update_warp_display();
}


void MainWindow::on_warpmax_clicked()
{
    universe_time_warp = universe_max_warp;
    update_warp_display();
}

void MainWindow::update_warp_display()
{
    if (universe_time_warp < 0)
        display_warp->setText("1/" + QString::number((1<<(-1*universe_time_warp))) + "x");
    else if (universe_time_warp < 10)
        display_warp->setText(QString::number((1<<(universe_time_warp))) + "x");
    else if (universe_time_warp < 20)
        display_warp->setText(QString::number((1<<(universe_time_warp-10))) + "kx");
    else if (universe_time_warp < 30)
        display_warp->setText(QString::number((1<<(universe_time_warp-20))) + "Mx");
}


void MainWindow::newsystemview()
{
    //TODO: maybe make these things track to viewed system name when they arent the parent window (or maybe even when they are?)
    MainWindow *w = new MainWindow(this);
    w->setWindowTitle("System View");
    w->show();
}
