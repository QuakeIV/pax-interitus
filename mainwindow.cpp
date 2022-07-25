#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "universe.h"
#include "systemwindow.h"
#include "techwindow.h"
#include <QWidget>

//TODO: temporary
#include "designer_windows/capacitordesigner.h"
#include "designer_windows/spacecraftdesigner.h"

// NOTE: not meant to be more than one of these
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openGL = this->findChild<SystemRenderer*>("systemview");
    pause_button = this->findChild<QPushButton*>("pausebutton");
    display_warp = this->findChild<QPushButton*>("displaywarp");
    status_bar = this->findChild<QStatusBar*>("statusbar");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::update_status_bar);
    // TODO: configurable framerate (although this is just updating the status bar)
    timer->start(50);

    // main timer driving the universe sim (should roughly match frame rate)
    // TODO: this is kindof sloppy but should mostly work fine
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout,  [timer]() {universe_update(MILLISECONDS_TO_TIME(timer->interval()));});
    //circa 60 fps? TODO: maybe better frame time mechanism? probably not particularly vital to gun for accurate times
    timer->start(1000.0/60.0);

    //Qt::WindowStaysOnBottomHint Qt::X11BypassWindowManagerHint
    //setWindowFlags(Qt::WindowStaysOnBottomHint);
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
    if (universe_time_warp <= -20)
        display_warp->setText("1/" + QString::number((1<<(-1*universe_time_warp - 20))) + "Mx");
    else if (universe_time_warp <= -10)
        display_warp->setText("1/" + QString::number((1<<(-1*universe_time_warp - 10))) + "kx");
    else if (universe_time_warp < 0)
        display_warp->setText("1/" + QString::number((1<<(-1*universe_time_warp))) + "x");
    else if (universe_time_warp < 10)
        display_warp->setText(QString::number((1<<(universe_time_warp))) + "x");
    else if (universe_time_warp < 20)
        display_warp->setText(QString::number((1<<(universe_time_warp-10))) + "kx");
    else if (universe_time_warp < 30)
        display_warp->setText(QString::number((1<<(universe_time_warp-20))) + "Mx");
}

void MainWindow::update_status_bar()
{
    QString message = "Time: " + get_date_str();
    status_bar->showMessage(message);
}

void MainWindow::on_actionSystemWindow_triggered()
{
    //TODO: maybe make these things track to viewed system name when they arent the parent window (or maybe even when they are?)
    //MainWindow *w = new MainWindow(this); // NOTE: it is possible to spawn multiple main windows, but im tending away from that at this point
    SystemWindow *w = new SystemWindow(openGL->get_focus_system(), this);
    w->show();
}

void MainWindow::on_actionTechWindow_triggered()
{
    TechWindow *w = new TechWindow(this);
    w->show();
}

void MainWindow::on_actionSpacecraftDesigner_triggered()
{
    SpacecraftDesigner *w = new SpacecraftDesigner(this);
    w->show();
}
