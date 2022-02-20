#include "capacitordesigner.h"
#include "ui_capacitordesigner.h"
#include "universe.h"
#include "utilities.h"

// TODO: put in common area somewhere?
class Qint64Validator : public QValidator
{
    int64_t min;
    int64_t max;

public:
    Qint64Validator(int64_t mi, int64_t ma, QObject *parent = nullptr)
    {
        min = mi;
        max = ma;
    }

    QValidator::State validate(QString &input, int &pos) const override
    {
        if (input == "")
            return QValidator::Intermediate;

        if (input[0] == '-')
        {
            if (min > 0)
                return QValidator::Invalid;
        }
        else
        {
            if (max < 0)
                return QValidator::Invalid;
        }

        bool ok = false;
        int64_t val = input.toLong(&ok);
        if (ok)
        {
            if (val < max && val > min)
                return QValidator::Acceptable;
            else
                return QValidator::Intermediate;
        }
        else
            return QValidator::Invalid;

        return QValidator::Intermediate;
    }
};

CapacitorDesigner::CapacitorDesigner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CapacitorDesigner)
{
    ui->setupUi(this);

    setWindowTitle("Capacitor Designer");

    // TODO: this will need more logic later
    // (maybe the empire remembers the last selected dialectric,
    // in addition to this polling known materials rather than
    // all materials ever, and also respecting hidden/obsolete
    // materials)
    selected_dialectric = &dialectric_materials[0];

    // capture pointers for UI elements
    dialectric_combobox = this->findChild<QComboBox*>("dialectric");
    resistance_edit = this->findChild<QLineEdit*>("resistance_edit");
    area_edit = this->findChild<QLineEdit*>("area_edit");
    separation_edit = this->findChild<QLineEdit*>("separation_edit");
    details = this->findChild<QLabel*>("details");

    // set validators
    // (TODO: this probably needs a Qint64 validator, additionally we may need to write that)
    resistance_edit->setValidator(new Qint64Validator(1, LONG_MAX, resistance_edit));
    area_edit->setValidator(new Qint64Validator(1, LONG_MAX, area_edit));
    separation_edit->setValidator(new Qint64Validator(1, LONG_MAX, separation_edit));

    // capture combobox changes
    connect(dialectric_combobox, QOverload<int>::of(&QComboBox::activated),
        [=](int index)
    {
        if (index >= 0 && index < dialectric_materials.length())
            this->selected_dialectric = &dialectric_materials[index];
        this->update();
    });

    // capture resistance changes
    connect(resistance_edit, QOverload<const QString &>::of(&QLineEdit::textEdited),
        [=](const QString &text)
    { this->update(); });

    // capture area changes
    connect(area_edit, QOverload<const QString &>::of(&QLineEdit::textEdited),
        [=](const QString &text)
    { this->update(); });

    // capture separation changes
    connect(separation_edit, QOverload<const QString &>::of(&QLineEdit::textEdited),
        [=](const QString &text)
    { this->update(); });

    update();
}

// refresh display to capture new world state
void CapacitorDesigner::update(void)
{
    // update dialectric display
    dialectric_combobox->clear();
    // TODO: this should iterate on an empire's knowledge of dialectrics instead
    foreach(Dialectric d, dialectric_materials)
        dialectric_combobox->addItem(d.descriptor_string());
    //annoyingly this has to match one of the existing items exactly
    dialectric_combobox->setCurrentText(selected_dialectric->descriptor_string());

    design.dialectric = selected_dialectric;
    design.resistance = resistance_edit->text().toLong();
    design.plate_area = area_edit->text().toLong();
    design.plate_separation = separation_edit->text().toLong();

    int64_t max_voltage = design.max_voltage();
    QString s = "Max Voltage: " + get_voltage_str(max_voltage);
    s += "\nPeak Amperage: " + get_amperage_str(design.max_current(max_voltage));
    s += "\nCapacitance: " + get_capacitance_str(design.capacitance());
    s += "\nEnergy Storage (at max voltage): " + get_energy_str(design.max_energy(max_voltage));
    s += "\nTime to Full Charge: " + get_time_str(design.charge_time());
    details->setText(s);
}

CapacitorDesigner::~CapacitorDesigner()
{
    delete ui;
}

//void CapacitorDesigner::changeEvent(QEvent *event)
//{
//    qDebug() << "lul";
//}

// TODO: this may turn out to be by far the best place to trigger updates
void CapacitorDesigner::enterEvent(QEnterEvent *event)
{
    update();
}
