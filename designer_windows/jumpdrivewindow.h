#ifndef JUMPDRIVEWINDOW_H
#define JUMPDRIVEWINDOW_H

#include "ui_jumpdrivewindow.h"
#include "spacecraft/spacecraft.h"
#include "components/jumpdrive.h"
#include <QMainWindow>
#include <QInputDialog>

class JumpdriveWindow : public QMainWindow
{
    Q_OBJECT

    JumpdriveDesign *drivedesign = nullptr;

    QPushButton *circuitadd;
    QPushButton *circuitremove;
    QPushButton *circuitname;
    QTextEdit   *circuittext;
    QLabel      *circuitnamelabel;

    void enterEvent(QEnterEvent *event) override
    {
        update();
    }

    QString drive_description(void)
    {
        if (!drivedesign)
            return "No design selected";
        QString str = drivedesign->name + " jump drive design spec";
        return str;
    }

    // update everything to reflect any new state in the window or the world
    void update(void)
    {
        setWindowTitle("Jump Drive Window");

        circuittext->setText(drive_description());

        if (drivedesign)
            circuitnamelabel->setText(drivedesign->name);
        else
            circuitnamelabel->setText("No Drive Selected");
    }

public:
    explicit JumpdriveWindow(QWidget *parent = nullptr):
        QMainWindow(parent),
        ui(new Ui::JumpdriveWindow)
    {
        ui->setupUi(this);

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
    ~JumpdriveWindow()
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
        if (!drivedesign)
            return;

        bool ok;
        QString text = QInputDialog::getText(this, "Design Name", "Name", QLineEdit::Normal, drivedesign->name, &ok);
        if (ok && !text.isEmpty())
        {
            drivedesign->name = text;
            update();
        }
    }

private:
    Ui::JumpdriveWindow *ui;
};

#endif // JUMPDRIVEWINDOW_H

