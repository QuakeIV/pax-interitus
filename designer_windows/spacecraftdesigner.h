#ifndef SPACECRAFTDESIGNER_H
#define SPACECRAFTDESIGNER_H

#include "ui_spacecraftdesigner.h"
#include "spacecraft.h"
#include <QMainWindow>
#include <QTreeView>

//typedef enum circuitview_type_e
//{
//    circuit_type   = QTreeWidgetItem::UserType,
//    component_type = QTreeWidgetItem::UserType + 1,
//} circuitview_type_t;

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parent = nullptr)
        : m_itemData(data), m_parentItem(parent)
    {}
    ~TreeItem()
    {
        qDeleteAll(m_childItems);
    }

    void appendChild(TreeItem *child)
    {
        m_childItems.append(child);
    }

    TreeItem *child(int row)
    {
        if (row < 0 || row >= m_childItems.size())
            return nullptr;
        return m_childItems.at(row);
    }
    int childCount() const
    {
        return m_childItems.count();
    }
    int columnCount() const
    {
        return 3;
    }
    QVariant data(int column) const
    {
        if (column < 0 || column >= m_itemData.size())
            return QVariant();
        return m_itemData.at(column);
    }
    int row() const
    {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

        return 0;
    }
    TreeItem *parentItem()
    {
        return m_parentItem;
    }

private:
    QList<TreeItem *> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QObject *parent = nullptr)
        : QAbstractItemModel(parent)
    {
        rootItem = new TreeItem({tr("Title"), tr("Summary")});
    }
    ~TreeModel()
    {
        delete rootItem;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole)
            return QVariant();

        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

        return item->data(index.column());
    }
    Qt::ItemFlags flags(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return QAbstractItemModel::flags(index);
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return rootItem->data(section);

        return QVariant();
    }
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        TreeItem *parentItem;

        if (!parent.isValid())
            parentItem = rootItem;
        else
            parentItem = static_cast<TreeItem*>(parent.internalPointer());

        TreeItem *childItem = parentItem->child(row);
        if (childItem)
            return createIndex(row, column, childItem);
        return QModelIndex();
    }
    QModelIndex parent(const QModelIndex &index) const override
    {
        if (!index.isValid())
            return QModelIndex();

        TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
        TreeItem *parentItem = childItem->parentItem();

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
    }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        TreeItem *parentItem;
        if (parent.column() > 0)
            return 0;

        if (!parent.isValid())
            parentItem = rootItem;
        else
            parentItem = static_cast<TreeItem*>(parent.internalPointer());

        return parentItem->childCount();
    }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        if (parent.isValid())
            return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
        return rootItem->columnCount();
    }

private:
    TreeItem *rootItem;
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

