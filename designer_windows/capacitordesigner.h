#ifndef CAPACITORDESIGNER_H
#define CAPACITORDESIGNER_H

#include <QDialog>
#include "components/capacitor.h"
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class CapacitorDesigner;
}

class CapacitorDesigner : public QDialog
{
    Q_OBJECT

    CapacitorDesign design;
    Dialectric *selected_dialectric;
    QComboBox *dialectric_combobox;
    QLineEdit *resistance_edit;
    QLineEdit *area_edit;
    QLineEdit *separation_edit;
    QLabel *details;

//    void changeEvent(QEvent *event) override;
    void enterEvent(QEnterEvent *event) override;

    // update everything to reflect new world state or other updates
    void update(void);

public:
    explicit CapacitorDesigner(QWidget *parent = nullptr);
    ~CapacitorDesigner();

private:
    Ui::CapacitorDesigner *ui;
};

#endif // CAPACITORDESIGNER_H
