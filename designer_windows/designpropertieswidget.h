#ifndef DESIGNPROPERTIESWIDGET_H
#define DESIGNPROPERTIESWIDGET_H

#include <QWidget>

namespace Ui {
class DesignPropertiesWidget;
}

class DesignPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DesignPropertiesWidget(QWidget *parent = nullptr);
    ~DesignPropertiesWidget();

private:
    Ui::DesignPropertiesWidget *ui;
};

#endif // DESIGNPROPERTIESWIDGET_H
