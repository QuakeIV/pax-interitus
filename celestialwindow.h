#ifndef CELESTIALWINDOW_H
#define CELESTIALWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

class CelestialType;

namespace Ui {
class CelestialWindow;
}

class CelestialWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CelestialWindow(CelestialType *c, QWidget *parent = nullptr);
    ~CelestialWindow();

    CelestialType *celestial;

    QPlainTextEdit *text;

public slots:
    void refresh(void);

private slots:
    void on_copytextview_clicked();

private:
    Ui::CelestialWindow *ui;
};

#endif // CELESTIALWINDOW_H
