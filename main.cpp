#include "mainwindow.h"

#include <QApplication>
#include <QRandomGenerator>

QApplication *qapp;
QRandomGenerator qrand;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qapp = &a;
    MainWindow w;
    w.show();
    return a.exec();
}
