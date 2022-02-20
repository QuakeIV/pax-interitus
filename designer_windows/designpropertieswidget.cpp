#include "designpropertieswidget.h"
#include "ui_designpropertieswidget.h"

DesignPropertiesWidget::DesignPropertiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DesignPropertiesWidget)
{
    ui->setupUi(this);
}

DesignPropertiesWidget::~DesignPropertiesWidget()
{
    delete ui;
}
