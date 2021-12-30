#include "celestialwindow.h"
#include "ui_celestialwindow.h"
#include <QTimer>
#include "celestialtype.h"
#include <QClipboard>

CelestialWindow::CelestialWindow(CelestialType *c, QWidget *parent) :
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
    s += "radius: " + QString::number(celestial->radius) + "\n";
    s += "orbital radius " + QString::number(celestial->trajectory.orbital_radius);
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

