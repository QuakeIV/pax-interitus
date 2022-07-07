#ifndef TECHWINDOW_H
#define TECHWINDOW_H

#include <QMainWindow>
#include "ui_techwindow.h"
#include "designer_windows/capacitordesigner.h"
#include "designer_windows/spacecraftdesigner.h"

namespace Ui {
class TechWindow;
}

class TechWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TechWindow(QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::TechWindow)
    {
        ui->setupUi(this);

        setWindowTitle("Technology Window");
    }

    ~TechWindow()
    {
        delete ui;
    }

private slots:

    void on_capacitordesigner_clicked()
    {
        CapacitorDesigner *d = new CapacitorDesigner(this);
        d->show();
    }

    void on_spacecraftdesigner_clicked()
    {
        SpacecraftDesigner *d = new SpacecraftDesigner(this);
        d->show();
    }

private:
    Ui::TechWindow *ui;
};

#endif // TECHWINDOW_H
