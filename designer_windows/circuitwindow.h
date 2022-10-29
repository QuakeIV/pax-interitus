#ifndef CIRCUITWINDOW_H
#define CIRCUITWINDOW_H

#include "ui_circuitwindow.h"
#include "spacecraft/spacecraft.h"
#include "components/circuit.h"
#include <QMainWindow>
#include <QInputDialog>

class CircuitWindow : public QMainWindow
{
    Q_OBJECT

    SpacecraftDesign *spacecraftdesign;
    // currently selected circuit, this window also points to the actual spacecraft design
    CircuitDesign *circuitdesign = nullptr;

    QPushButton *circuitadd;
    QPushButton *circuitremove;
    QPushButton *circuitname;
    QTextEdit   *circuittext;
    QLabel      *circuitnamelabel;

    void enterEvent(QEnterEvent *event) override
    {
        update();
    }

    QString circuit_description(void)
    {
        if (!circuitdesign)
            return "No design selected";
        QString str = circuitdesign->name + " circuit design spec";
        return str;
    }

    // update everything to reflect any new state in the window or the world
    void update(void)
    {
        // handle case where design was deleted
        if (spacecraftdesign && !spacecraft_designs.contains(spacecraftdesign))
        {
            close();
            return;
        }

        setWindowTitle("Power Circuit Window (" + get_spacecraft_design_name(spacecraftdesign) + ")");

        circuittext->setText(circuit_description());

        if (circuitdesign)
            circuitnamelabel->setText(circuitdesign->name);
        else
            circuitnamelabel->setText("No Circuit Selected");
    }

public:
    explicit CircuitWindow(SpacecraftDesign *d, QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::CircuitWindow)
    {
        ui->setupUi(this);

        spacecraftdesign = d;

        // capture pointers for UI elements
        circuitadd       = this->findChild<QPushButton*>("circuitadd");
        circuitremove    = this->findChild<QPushButton*>("circuitremove");
        circuitname      = this->findChild<QPushButton*>("circuitname");
        circuittext      = this->findChild<QTextEdit*>("circuittext");
        circuitnamelabel = this->findChild<QLabel*>("circuitnamelabel");

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
    ~CircuitWindow()
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

    void on_circuitname_clicked()
    {
        if (!circuitdesign)
            return;

        bool ok;
        QString text = QInputDialog::getText(this, "Design Name", "Name", QLineEdit::Normal, circuitdesign->name, &ok);
        if (ok && !text.isEmpty())
        {
            circuitdesign->name = text;
            update();
        }
    }

private:
    Ui::CircuitWindow *ui;
};

#endif // CIRCUITWINDOW_H

