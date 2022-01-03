#ifndef SYSTEMWINDOW_H
#define SYSTEMWINDOW_H

#include <QMainWindow>
#include "solarsystemtype.h"

namespace Ui {
class SystemWindow;
}

class SystemWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SystemWindow(SolarSystemType *system, QWidget *parent);
    ~SystemWindow();

private:
    Ui::SystemWindow *ui;
};

#endif // SYSTEMWINDOW_H
