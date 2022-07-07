#ifndef SPACECRAFTDESIGNER_H
#define SPACECRAFTDESIGNER_H

#include "ui_spacecraftdesigner.h"
#include "spacecraft.h"
#include <QMainWindow>

class SpacecraftDesigner : public QMainWindow
{
    Q_OBJECT

    SpacecraftDesign design;

//    void changeEvent(QEvent *event) override;
    void enterEvent(QEnterEvent *event) override
    {
        update();
    }

    // update everything to reflect new world state or other updates
    void update(void)
    {
    }

public:
    explicit SpacecraftDesigner(QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::SpacecraftDesigner)
    {
        ui->setupUi(this);

        // capture pointers for UI elements
//        dialectric_combobox = this->findChild<QComboBox*>("dialectric");

        // TODO: maybe assert on the pointers to the widgets, the below segfaults if the above findChild calls fail
        // capture combobox changes
//        connect(dialectric_combobox, QOverload<int>::of(&QComboBox::activated),
//            [=](int index)
//        {
//            if (index >= 0 && index < dialectric_materials.length())
//                this->selected_dialectric = &dialectric_materials[index];
//            this->update();
//        });

        update();
    }
    ~SpacecraftDesigner()
    {
        delete ui;
    };

private:
    Ui::SpacecraftDesigner *ui;
};

#endif // SPACECRAFTDESIGNER_H

