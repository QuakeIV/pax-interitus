#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class SystemRenderer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// NOTE: not meant to be more than one of these
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newsystemwindow(void);
    void newtechwindow(void);

    void update_status_bar(void);

private slots:
    void on_systemlist_doubleClicked(const QModelIndex &index);

    void on_pausebutton_clicked(bool checked);

    void on_minuswarp_clicked();

    void on_pluswarp_clicked();

    void on_warp1_clicked();

    void on_warpmax_clicked();

    void on_warp1k_clicked();

    void on_warp1M_clicked();

private:
    Ui::MainWindow *ui;
    SystemRenderer *openGL;
    QPushButton *pause_button;
    QPushButton *display_warp;
    QStatusBar *status_bar;

    void update_warp_display(void);
};
#endif // MAINWINDOW_H
