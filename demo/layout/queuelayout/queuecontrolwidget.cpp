#include "queuecontrolwidget.h"
#include "ui_queuecontrolwidget.h"

#include <QtCore/qdebug.h>


QueueControlWidget::QueueControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueueControlWidget)
{
    ui->setupUi(this);

    setWindowTitle("demo - queue layout");

    videosWidget.installEventFilter(this);

    ui->cb_align->addItem("Left", Qt::AlignLeft);
    ui->cb_align->addItem("Right", Qt::AlignRight);
    ui->cb_align->addItem("Top", Qt::AlignTop);
    ui->cb_align->addItem("Bottom", Qt::AlignBottom);
    ui->cb_align->addItem("HCenter", Qt::AlignHCenter);
    ui->cb_align->addItem("VCenter", Qt::AlignVCenter);
    ui->cb_align->addItem("Center", Qt::AlignCenter);
    connect(ui->cb_align, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            [this](int index){
        videosWidget.setAlignment(static_cast<Qt::AlignmentFlag>(ui->cb_align->itemData(index).toInt()));
    });
    videosWidget.setAlignment(static_cast<Qt::AlignmentFlag>(ui->cb_align->currentData().toInt()));

    connect(ui->push, &QPushButton::clicked, [this](){
        videosWidget.push();
    });
    connect(ui->pop, &QPushButton::clicked, [this](){
        videosWidget.pop();
    });
    connect(ui->insert, &QPushButton::clicked, [this](){
        videosWidget.insert(ui->lineEdit_insert_row->text().toInt()-1,
                            ui->lineEdit_insert_col->text().toInt()-1);
    });
    connect(ui->remove, &QPushButton::clicked, [this](){
        videosWidget.remove(ui->lineEdit_remove_row->text().toInt()-1,
                            ui->lineEdit_remove_col->text().toInt()-1);
    });

    connect(ui->show, &QPushButton::clicked, [this](){
        videosWidget.setVisible(! videosWidget.isVisible());
        switchWidgets(videosWidget.isVisible());
    });
    videosWidget.hide();
    switchWidgets(false);
}

QueueControlWidget::~QueueControlWidget()
{
    delete ui;
}

bool QueueControlWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == &videosWidget
            && event->type() == QEvent::Close) {
        switchWidgets(false);
    }

    return QWidget::eventFilter(target, event);
}

void QueueControlWidget::switchWidgets(bool value)
{
    ui->cb_align->setEnabled(value);
    ui->push->setEnabled(value);
    ui->pop->setEnabled(value);
    ui->insert->setEnabled(value);
    ui->remove->setEnabled(value);
    ui->lineEdit_insert_row->setEnabled(value);
    ui->lineEdit_insert_col->setEnabled(value);
    ui->lineEdit_remove_row->setEnabled(value);
    ui->lineEdit_remove_col->setEnabled(value);
}
