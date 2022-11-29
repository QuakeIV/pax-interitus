#ifndef CELESTIALWINDOW_H
#define CELESTIALWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include "ui_celestialwindow.h"
#include <QTimer>
#include "celestial.h"
#include <QClipboard>
#include "utilities.h"

namespace Ui {
class CelestialWindow;
}

class CelestialWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CelestialWindow(Celestial *c, QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::CelestialWindow)
    {
        ui->setupUi(this);

        celestial = c;

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &CelestialWindow::refresh);
        // TODO: fiddle with update rate, make configurable with refresh/frame rate?
        timer->start(50);

        text = this->findChild<QTextEdit*>("textview");

        setWindowTitle(celestial->name);
    }

    ~CelestialWindow()
    {
        delete ui;
    }

    Celestial *celestial;

    QTextEdit *text;

public slots:
    void refresh(void)
    {
        // for now just use the text window as a cheapo way to display info until things settle down
        QString s = celestial->name + ":\n";
        s += "x: " + QString::number(celestial->trajectory.position.x) + "\n";
        s += "y: " + QString::number(celestial->trajectory.position.y) + "\n";
        s += "mass: " + QString::number(celestial->mass) + "\n";
        s += "radius: " + get_distance_str(DISTANCE_FIXED_TO_FLOAT(celestial->radius)) + "\n";
        s += "orbital radius: " + get_distance_str(DISTANCE_FIXED_TO_FLOAT(celestial->trajectory.orbital_radius));
        if (celestial->trajectory.parent)
        {
            // test v2d shittery
            s += "\ndist from primary: " + get_distance_str(DISTANCE_FIXED_TO_FLOAT((celestial->trajectory.position - celestial->trajectory.parent->trajectory.position).length()));
            s += "\nrootfinder error: " + get_distance_str(DISTANCE_FIXED_TO_FLOAT((celestial->trajectory.position - celestial->trajectory.parent->trajectory.position).length() - celestial->trajectory.orbital_radius));
            s += "\nNOTE: error should go to 1 or 0 on the racetrack nodes";
        }
        else
        {
            s += "\ndist from primary: " + get_distance_str(DISTANCE_FIXED_TO_FLOAT(celestial->trajectory.position.length()));
        }
        text->document()->setPlainText(s);
    }

private slots:
    void on_copytextview_clicked()
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(text->document()->toPlainText());
    }

private:
    Ui::CelestialWindow *ui;
};

#endif // CELESTIALWINDOW_H
