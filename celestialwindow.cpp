#include "celestialwindow.h"
#include "ui_celestialwindow.h"
#include <QTimer>
#include "celestialtype.h"
#include <QClipboard>
#include "utilities.h"

CelestialWindow::CelestialWindow(CelestialType *c, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::CelestialWindow)
{
    ui->setupUi(this);

    celestial = c;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CelestialWindow::refresh);
    // TODO: fiddle with update rate
    timer->start(50);

    text = this->findChild<QPlainTextEdit*>("textview");

    setWindowTitle(celestial->name);
}

void CelestialWindow::refresh()
{
    // for now just use the text window as a cheapo way to display info until things settle down
    QString s = celestial->name + ":\n";
    s += "x: " + QString::number(celestial->trajectory.position.x) + "\n";
    s += "y: " + QString::number(celestial->trajectory.position.y) + "\n";
    s += "mass: " + QString::number(celestial->mass) + "\n";
    s += "radius: " + get_distance_str(celestial->radius) + "\n";
    s += "orbital radius: " + get_distance_str(celestial->trajectory.orbital_radius);
    // test v2d shittery
    s += "\ndist from primary: " + get_distance_str(celestial->trajectory.position.length());
    if (celestial->trajectory.parent)
    {
        s += "\nrootfinder error: " + get_distance_str((celestial->trajectory.position - celestial->trajectory.parent->trajectory.position).length() - celestial->trajectory.orbital_radius);
        s += "\nerror should go to 1 or 0 on the racetrack nodes";
    }
    text->document()->setPlainText(s);
}

CelestialWindow::~CelestialWindow()
{
    delete ui;
}

void CelestialWindow::on_copytextview_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text->document()->toPlainText());
}

