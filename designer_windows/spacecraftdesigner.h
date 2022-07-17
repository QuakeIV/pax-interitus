#ifndef SPACECRAFTDESIGNER_H
#define SPACECRAFTDESIGNER_H

#include "ui_spacecraftdesigner.h"
#include "spacecraft.h"
#include <QMainWindow>
#include <QTreeView>

class CircuitTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    CircuitTreeModel(QObject *parent = nullptr):
        QAbstractItemModel(parent)
    {
    }
    ~CircuitTreeModel() {}

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();
        return QVariant("soy" + QString::number(index.row()));
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if (role != Qt::DisplayRole)
            return QVariant();

        switch(section)
        {
            case 0:
                return QVariant("Name");
            case 1:
                return QVariant("Power (W)");
            case 2:
                return QVariant("Spec Voltage");
            case 3:
                return QVariant("Spec Amperage");
        }

        return QVariant();
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        // TODO: range check needed? unclear
//        qDebug() << "x: " << row << " y: " << column;
        return createIndex(row, column);
    }
    QModelIndex parent(const QModelIndex &index) const override
    {
        qDebug() << "apparent";
        if (!index.isValid())
            return QModelIndex();

        // TODO: createIndex to circuit entry if iterating over actual circuit children
        return QModelIndex();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        //TODO: actual real version of this
        if (parent.column() > 0)
            return 0;
        if (!parent.isValid())
            return 20;

        return 0;
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        return 4; // name, power, voltage, current
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return Qt::NoItemFlags;
        // TODO: only allow circuit voltage/amperage to be edited?
        return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled; // Qt::ItemIsEditable |
    }
//    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
//    {
//        return true;
//    }
//    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override
    {
        qDebug() << "move";
        return true;
    }
};

class SpacecraftDesigner : public QMainWindow
{
    Q_OBJECT

    SpacecraftDesign design;

    QTreeView *circuitview;

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
        circuitview = this->findChild<QTreeView*>("circuitview");
        CircuitTreeModel *model = new CircuitTreeModel();
        circuitview->setModel(model);

        // TODO: maybe assert on the pointers to the widgets, the below segfaults if the above findChild calls fail
        // capture circuit view changes
        connect(circuitview, QOverload<const QModelIndex &>::of(&QTreeView::activated),
            [=](const QModelIndex &index)
        { this->update(); });

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

