#include "mainwindow.h"

#include <QApplication>

QApplication *qapp;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qapp = &a;
    MainWindow w;
    w.show();
    return a.exec();
}
