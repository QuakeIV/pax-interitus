#include "mainwindow.h"

#include <QApplication>
#include <QRandomGenerator>
#include "universe.h"

QApplication *qapp;
QRandomGenerator qrand; //TODO: figure out how to key this little shit off of the clock so its not procedural

int main(int argc, char *argv[])
{
    universe_init();

    QApplication a(argc, argv);
    qapp = &a;
    MainWindow w;
    w.show();
    return a.exec();
}
