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
    TreeItem(const QList<QVariant> &data = {})
        : m_itemData(data)
    {
    }
    ~TreeItem()
    {
    }

    bool operator==(const TreeItem *rhs) const
    {
        return rhs == const_cast<TreeItem*>(this);
    }

    TreeItem *appendChild(TreeItem child)
    {
        int idx = m_childItems.count();
        m_childItems.append(child);
        m_childItems[idx].m_parentItem = this;
        return const_cast<TreeItem*>(&m_childItems[idx]);
    }
    TreeItem *child(int row)
    {
        if (row < 0 || row >= m_childItems.size())
            return nullptr;
        return const_cast<TreeItem*>(&m_childItems.at(row));
    }
    int childCount() const
    {
        return m_childItems.count();
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
            return m_parentItem->m_childItems.indexOf(this);

        return 0;
    }
    TreeItem *parentItem()
    {
        return m_parentItem;
    }

    TreeItem *m_parentItem;
private:
    QList<TreeItem> m_childItems;
    QList<QVariant> m_itemData;
};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeItem rootItem;

    TreeModel(QObject *parent = nullptr)
        : QAbstractItemModel(parent), rootItem()
    {
    }
    ~TreeModel() {}

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

        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        // TODO: feels jank, no need to range check?
        static QString poop[] = {"Head1", "head2", "poop"};
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return poop[section];

        return QVariant();
    }
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
        if (!hasIndex(row, column, parent))
            return QModelIndex();

        TreeItem *parentItem;

        // TODO: why the hell is a const cast needed?
        if (!parent.isValid())
            parentItem = const_cast<TreeItem*>(&rootItem);
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

        if (parentItem == &rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
    }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        TreeItem *parentItem;
        if (parent.column() > 0)
            return 0;

        // TODO: why the hell is a const cast needed?
        if (!parent.isValid())
            parentItem = const_cast<TreeItem*>(&rootItem);
        else
            parentItem = static_cast<TreeItem*>(parent.internalPointer());

        return parentItem->childCount();
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

        TreeModel *tree = new TreeModel();
        circuitview->setModel(tree);

        TreeItem *item0 = tree->rootItem.appendChild(TreeItem({"gay", "fay", "way"}));
        TreeItem *item1 = item0->appendChild(TreeItem({"chay", "stay", "dead"}));
        item0->appendChild(TreeItem({"a", "b", "c"}));
        TreeItem *item2 = tree->rootItem.appendChild(TreeItem({"shit", "fit", "crit"}));
        item2->appendChild(TreeItem({"trigger", "wigger", "n"}));

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

