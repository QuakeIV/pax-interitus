#ifndef SPACECRAFTDESIGNER_H
#define SPACECRAFTDESIGNER_H

#include "ui_spacecraftdesigner.h"
#include "spacecraft/spacecraft.h"
#include "circuitwindow.h"
#include <QMainWindow>
#include <QInputDialog>

class SpacecraftDesigner : public QMainWindow
{
    Q_OBJECT

    SpacecraftDesign *design = nullptr;

    QPushButton *circuitwindow;
    QTextEdit   *designtext;
    QPushButton *designname;
    QComboBox   *selecteddesign;

    void enterEvent(QEnterEvent *event) override
    {
        update();
    }

    QString design_description(void)
    {
        if (!design)
            return "No design selected";
        QString str = "";

        str += get_spacecraft_design_name(design);

        return str;
    }

    // update everything to reflect any new state in the window or the world
    void update(void)
    {
        // handle case where design was deleted
        if (design && !spacecraft_designs.contains(design))
            design = nullptr;

        designtext->setText(design_description());

        if (design)
            setWindowTitle("Spacecraft Designer (" + get_spacecraft_design_name(design) + ")");
        else
            setWindowTitle("Spacecraft Designer");

        // update design list
        selecteddesign->clear();
        qsizetype idx = 0;
        // TODO: this should iterate on an empire's list of designs instead
        // iterating backwards for now, feels more ergonomic with respect to everything else
        for (int i = spacecraft_designs.length() - 1; i >= 0; i--)
        {
            SpacecraftDesign *d = spacecraft_designs[i];
            selecteddesign->addItem(get_spacecraft_design_name(d), (qulonglong)d);
            if (d == design) //if no match, de facto we get left at 0 (the top item)
                idx = spacecraft_designs.length() - i - 1; // subtracting since we are iterating backwards, -1 to avoid going off end of list
        }
        selecteddesign->setCurrentIndex(idx);
    }

public:
    explicit SpacecraftDesigner(QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::SpacecraftDesigner)
    {
        ui->setupUi(this);

        // capture pointers for UI elements
        circuitwindow  = this->findChild<QPushButton*>("circuitwindow");
        designtext     = this->findChild<QTextEdit*>("designtext");
        designname     = this->findChild<QPushButton*>("designname");
        selecteddesign = this->findChild<QComboBox*>("selecteddesign");

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
    void on_circuitwindow_clicked()
    {
        if (!design)
            return;

        CircuitWindow *c = new CircuitWindow(design, this);
        c->show();
        update();
    }

    void on_designname_clicked()
    {
        if (!design)
            return;

        bool ok;
        QString text = QInputDialog::getText(this, "Design Name", "Name", QLineEdit::Normal, design->class_name, &ok);
        if (ok && !text.isEmpty())
        {
            design->class_name = text;
            update();
        }
    }

    void on_deletedesign_clicked()
    {
        if (!design)
        {
            update();
            return;
        }

        qsizetype next_index = spacecraft_designs.indexOf(design);

        delete design;
        spacecraft_designs.removeOne(design);
        if (spacecraft_designs.empty())
        {
            design = nullptr;
            update();
            return;
        }

        qsizetype len = spacecraft_designs.length();
        if (next_index >= len)
            next_index = len - 1;
        design = spacecraft_designs[next_index];
        update();
    }

    void on_newdesign_clicked()
    {
        SpacecraftDesign *d = new SpacecraftDesign();
        spacecraft_designs.append(d);

        design = d;
        update();
    }

    void on_showobsolete_clicked()
    {

    }

    void on_selecteddesign_activated(int index)
    {
        if (!selecteddesign->count())
        {
            design = nullptr;
            return;
        }

        SpacecraftDesign *d = (SpacecraftDesign *)selecteddesign->currentData().toULongLong();
        if (spacecraft_designs.contains(d))
            design = d;
        else
            design = nullptr;
        update();
    }

private:
    Ui::SpacecraftDesigner *ui;
};

#endif // SPACECRAFTDESIGNER_H

