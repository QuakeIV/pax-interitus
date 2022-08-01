#ifndef SPACECRAFTDESIGNER_H
#define SPACECRAFTDESIGNER_H

#include "ui_spacecraftdesigner.h"
#include "spacecraft.h"
#include <QMainWindow>
#include <QTreeView>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    SpacecraftDesign *design;

    TreeModel(SpacecraftDesign *d, QObject *parent = nullptr)
        : QAbstractItemModel(parent)
    {
        design = d;
    }
    ~TreeModel() {}

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole)
            return QVariant();

        if (design->circuits.contains((CircuitDesign *)index.internalPointer()))
        {
            CircuitDesign *circuit = (CircuitDesign *)index.internalPointer();
            switch (index.column())
            {
            case 0:
                return circuit->descriptor_string();
            case 1:
                return circuit->rated_amperage;
            case 2:
                return circuit->rated_voltage;
            default:
                return QVariant();
            }
        }

        ComponentDesign *component = (ComponentDesign *)index.internalPointer();

        switch(index.column())
        {
        case 0:
            return component->descriptor_string();
        case 1:
            return QVariant();
        case 2:
            return QVariant();
        default:
            return QVariant();
        }

        return QVariant();
    }
    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        // TODO: feels jank, no need to range check?
        static QString poop[] = {"Name", "Spec Current", "Spec Voltage"};
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return poop[section];

        return QVariant();
    }
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        // handle circuits
        if (!parent.isValid())
        {
            CircuitDesign *circuit = &design->circuits[row];
            return createIndex(row, column, circuit);
        }

        if (design->circuits.contains((CircuitDesign *)parent.internalPointer()))
        {
            CircuitDesign *parent_circuit = (CircuitDesign *)parent.internalPointer();
            ComponentDesign *component = parent_circuit->components[row];
            return createIndex(row, column, component);
        }

        return QModelIndex();
    }
    QModelIndex parent(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return QModelIndex();

        // TODO: verify we need to produce a parent index and that isnt handled above
        if (design->circuits.contains((CircuitDesign *)index.internalPointer()))
            return QModelIndex();

        // else case, we are in a component
        ComponentDesign *component = (ComponentDesign *)index.internalPointer();
        CircuitDesign *parent_circuit = component->circuit;
        int row = design->circuits.indexOf(parent_circuit);

        return createIndex(row, 0, parent_circuit);
    }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (parent.column() > 0)
            return 0;

        if (!parent.isValid())
            return design->circuits.count();

        if (design->circuits.contains((CircuitDesign *)parent.internalPointer()))
        {
            CircuitDesign *parent_circuit = (CircuitDesign *)parent.internalPointer();
            return parent_circuit->components.count();
        }

        return 0;
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return 3;
    }
};

class SpacecraftDesigner : public QMainWindow
{
    Q_OBJECT

    SpacecraftDesign design;

    QTreeView   *circuitview;
    QPushButton *circuitadd;
    QPushButton *circuitremove;

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
        circuitview   = this->findChild<QTreeView*>("circuitview");
        circuitadd    = this->findChild<QPushButton*>("circuitadd");
        circuitremove = this->findChild<QPushButton*>("circuitremove");

        TreeModel *tree = new TreeModel(&design);
        circuitview->setModel(tree);

        design.circuits.append(CircuitDesign());
        design.circuits.append(CircuitDesign());

        CircuitDesign *circuit0 = &design.circuits[0];
        CircuitDesign *circuit1 = &design.circuits[1];

        ReactorDesign *r1 = new ReactorDesign();
        ReactorDesign *r2 = new ReactorDesign();
        ReactorDesign *r3 = new ReactorDesign();
        ReactorDesign *r4 = new ReactorDesign();
        CapacitorDesign *c1 = new CapacitorDesign();
        CapacitorDesign *c2 = new CapacitorDesign();
        CapacitorDesign *c3 = new CapacitorDesign();
        CapacitorDesign *c4 = new CapacitorDesign();

        circuit0->components.append(r1);
        r1->circuit = circuit0;
        circuit0->components.append(r2);
        r2->circuit = circuit0;
        circuit0->components.append(c1);
        c1->circuit = circuit0;
        circuit0->components.append(c2);
        c2->circuit = circuit0;

        circuit1->components.append(r3);
        r3->circuit = circuit1;
        circuit1->components.append(r4);
        r4->circuit = circuit1;
        circuit1->components.append(c3);
        c3->circuit = circuit1;
        circuit1->components.append(c4);
        c4->circuit = circuit1;

//        QTreeWidgetItem *item = new QTreeWidgetItem({"Unassigned", "None", "None"});
//        item->setFlags(item->flags() & ~Qt::ItemIsDragEnabled);
//        circuitview->addTopLevelItem(item);
//        item->addChild(new QTreeWidgetItem({"Reactor", "100kV", "200A", ""}));
//        item->addChild(new QTreeWidgetItem({"Reactor", "200kV", "200A", ""}));

//        circuitview->setSelectionMode(QAbstractItemView::ExtendedSelection);
//        circuitview->setDragEnabled(true);
//        circuitview->viewport()->setAcceptDrops(true);
//        circuitview->setDropIndicatorShown(true);
//        circuitview->setDragDropMode(QAbstractItemView::DragDrop);
//        circuitview->setSelectionBehavior(QAbstractItemView::SelectRows);
//        circuitview->setDefaultDropAction(Qt::MoveAction);

        // TODO: maybe assert on the pointers to the widgets, the below segfaults if the above findChild calls fail
        // capture circuit view changes
        connect(circuitview, QOverload<const QModelIndex &>::of(&QTreeView::activated),
            [=](const QModelIndex &index)
        { this->update(); });

        circuitview->expandAll();

        update();
    }
    ~SpacecraftDesigner()
    {
        delete ui;
    }

private slots:
    void on_circuitadd_clicked()
    {

    }

    void on_circuitremove_clicked()
    {

    }

private:
    Ui::SpacecraftDesigner *ui;
};

#endif // SPACECRAFTDESIGNER_H

