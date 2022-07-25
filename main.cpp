#include "mainwindow.h"

#include <QApplication>
#include <QRandomGenerator>
#include "universe.h"
#include <QFile>

// TODO: fix, temporarily moving this to universe
extern QApplication *qapp;

int main(int argc, char *argv[])
{
    universe_init();

    QApplication a(argc, argv);
    qapp = &a;
    MainWindow w;

    // le icon
    Q_INIT_RESOURCE(images);
    qapp->setWindowIcon(QIcon(":/images/icon.png"));

    w.show();

    return a.exec();
}
