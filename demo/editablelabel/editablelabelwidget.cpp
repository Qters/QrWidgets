#include "editablelabelwidget.h"
#include "ui_editablelabelwidget.h"


USING_NS_QRWIDGETS;


EditableLabelWidget::EditableLabelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditableLabelWidget)
{
    ui->setupUi(this);

    ui->qters_editable_label->setEditable(true);
    ui->qters_editable_label->createLayout();
    ui->qters_editable_label->setText("qters editable label");

    this->setWindowTitle("demo - editable label");
}

EditableLabelWidget::~EditableLabelWidget()
{
    delete ui;
}
