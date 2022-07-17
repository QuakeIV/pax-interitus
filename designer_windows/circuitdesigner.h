#ifndef CIRCUITDESIGNER_H
#define CIRCUITDESIGNER_H

#include "ui_circuitdesigner.h"
#include <QDialog>
#include "components/circuit.h"
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

class CircuitDesigner : public QDialog
{
    Q_OBJECT

    CircuitDesign design;
    Insulator *selected_insulator;
    QComboBox *dialectric_combobox;
    QLineEdit *capacity_edit; // target capacity in joules
    QLineEdit *voltage_edit;
    QLineEdit *amperage_edit;
    QLabel *details;

//    void changeEvent(QEvent *event) override;
    void enterEvent(QEnterEvent *event) override
    {
        update();
    }

    // update everything to reflect new world state or other updates
    void update(void)
    {
        // TODO: support 100kJ notation

        // update dialectric display
//        dialectric_combobox->clear();
//        // TODO: this should iterate on an empire's knowledge of dialectrics instead
//        foreach(Dialectric d, dialectric_materials)
//            dialectric_combobox->addItem(d.descriptor_string());

//        // NOTE: annoyingly this has to match one of the existing items exactly
//        dialectric_combobox->setCurrentText(selected_dialectric->descriptor_string());

//        design.dialectric = selected_dialectric;

//        // TODO: spec voltage/amperage can probably change to a dropdown of established circuit specs
//        double spec_voltage = voltage_edit->text().toDouble();
//        // TODO: this needs to change if we ever introduce min separation
//        double separation = spec_voltage/design.dialectric->strength;
//        design.plate_separation = separation;

//        double spec_capacity = capacity_edit->text().toDouble();
//        double plate_area = (2 * spec_capacity * separation) / (spec_voltage * spec_voltage * design.dialectric->permittivity);
//        design.plate_area = plate_area;

//        double spec_amperage = amperage_edit->text().toDouble();
//        double resistance = spec_voltage/spec_amperage;
//        design.resistance = resistance;


//        QString s = "Capacitance: " + get_capacitance_str(design.capacitance());
//        s += "\nEnergy Storage (at spec voltage): " + get_energy_str(design.energy_at_voltage(spec_voltage));
//        s += "\nTime to Full Charge: " + get_time_str(design.charge_time());
//        s += "\nSpecced Plate Separation: " + get_distance_str(design.plate_separation);
//        s += "\nSpecced Inline Resistor: " + get_resistance_str(resistance);
//        s += "\nSpecced Area: " + get_area_str(plate_area);
//        s += "\nVolume: " + get_volume_str(design.volume());
////        s += "\nMass: " + get_mass_str(design.mass()); // TODO: need defined densities
//        s += "\nAmperage at spec voltage: " + get_amperage_str(design.max_current(spec_voltage));
//        s += "\nMax Design Voltage: " + get_voltage_str(design.max_voltage());
//        // TODO: document overall size of capacitor
//        details->setText(s);
    }

public:
    explicit CircuitDesigner(QWidget *parent = nullptr):
        QDialog(parent),
        ui(new Ui::CircuitDesigner)
    {
        ui->setupUi(this);

        setWindowFlags(Qt::Window); // enable taskbar icon, enable minimize

        // TODO: this will need more logic later
        // (maybe the empire remembers the last selected dialectric,
        // in addition to this polling known materials rather than
        // all materials ever, and also respecting hidden/obsolete
        // materials)
//        selected_dialectric = &dialectric_materials[0];

        // capture pointers for UI elements
        dialectric_combobox = this->findChild<QComboBox*>("dialectric");
        capacity_edit       = this->findChild<QLineEdit*>("capacity_edit");
        voltage_edit        = this->findChild<QLineEdit*>("voltage_edit");
        amperage_edit       = this->findChild<QLineEdit*>("amperage_edit");
        details             = this->findChild<QLabel*>("details");

        // set validators
        //resistance_edit->setValidator(new QDoubleValidator(1, ((double)LONG_MAX)/RESISTANCE_FACTOR, (RESISTANCE_SHIFT/10)*3, resistance_edit));
        //area_edit->setValidator(new QDoubleValidator(1, ((double)LONG_MAX)/PRECISION_AREA_FACTOR, (PRECISION_AREA_SHIFT/10)*3, area_edit));
        //separation_edit->setValidator(new QDoubleValidator(1, ((double)LONG_MAX)/PRECISION_DISTANCE_FACTOR, (PRECISION_DISTANCE_SHIFT/10)*3, separation_edit));

        // TODO: maybe assert on the pointers to the widgets, the below segfaults if the above findChild calls fail
        // capture combobox changes
//        connect(dialectric_combobox, QOverload<int>::of(&QComboBox::activated),
//            [=](int index)
//        {
//            if (index >= 0 && index < dialectric_materials.length())
//                this->selected_dialectric = &dialectric_materials[index];
//            this->update();
//        });

//        // capture area changes
//        connect(capacity_edit, QOverload<const QString &>::of(&QLineEdit::textEdited),
//            [=](const QString &text)
//        { this->update(); });

//        // capture voltage changes
//        connect(voltage_edit, QOverload<const QString &>::of(&QLineEdit::textEdited),
//            [=](const QString &text)
//        { this->update(); });

//        // capture amperage changes
//        connect(amperage_edit, QOverload<const QString &>::of(&QLineEdit::textEdited),
//            [=](const QString &text)
//        { this->update(); });

        update();
    }
    ~CircuitDesigner()
    {
        delete ui;
    };

private:
    Ui::CircuitDesigner *ui;
};

#endif // CIRCUITDESIGNER_H
