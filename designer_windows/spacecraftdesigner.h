#ifndef SPACECRAFTDESIGNER_H
#define SPACECRAFTDESIGNER_H

#include "ui_spacecraftdesigner.h"
#include "spacecraft/spacecraft.h"
#include <QMainWindow>
#include <QInputDialog>

class SpacecraftDesigner : public QMainWindow
{
    Q_OBJECT

    SpacecraftDesign design;

    QPushButton *circuitadd;
    QPushButton *circuitremove;
    QTextEdit   *designtext;
    QPushButton *designname;

    void enterEvent(QEnterEvent *event) override
    {
        update();
    }

    QString design_description(void)
    {
        QString str = design.class_name + " Class";
        return str;
    }

    // update everything to reflect any new state in the window or the world
    void update(void)
    {
        designtext->setText(design_description());
        if (design.class_name != "Spacecraft Design")
            setWindowTitle("Spacecraft Designer (" + design.class_name + " Class)");
    }

public:
    explicit SpacecraftDesigner(QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::SpacecraftDesigner)
    {
        ui->setupUi(this);

        // capture pointers for UI elements
        circuitadd    = this->findChild<QPushButton*>("circuitadd");
        circuitremove = this->findChild<QPushButton*>("circuitremove");
        designtext    = this->findChild<QTextEdit*>("designtext");
        designname    = this->findChild<QPushButton*>("designname");

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
        // updoot
//        connect(circuitview, QOverload<const QModelIndex &>::of(&QTreeView::activated),
//            [=](const QModelIndex &index)
//        { this->update(); });

        update();
    }
    ~SpacecraftDesigner()
    {
        delete ui;
    }

private slots:
    void on_circuitadd_clicked()
    {
        update();
    }

    void on_circuitremove_clicked()
    {
        update();
    }

    void on_designname_clicked()
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Design Name", "Name", QLineEdit::Normal, design.class_name, &ok);
        if (ok && !text.isEmpty())
        {
            design.class_name = text;
            update();
        }
    }

private:
    Ui::SpacecraftDesigner *ui;
};

#endif // SPACECRAFTDESIGNER_H

