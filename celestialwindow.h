#ifndef CELESTIALWINDOW_H
#define CELESTIALWINDOW_H

#include <QMainWindow>

namespace Ui {
class CelestialWindow;
}

class CelestialWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CelestialWindow(QWidget *parent = nullptr);
    ~CelestialWindow();

private:
    Ui::CelestialWindow *ui;
};

#endif // CELESTIALWINDOW_H
