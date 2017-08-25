#include "listviewexdemo.h"
#include "ui_listviewexdemo.h"

#include "tablistviewwidget.h"
#include "listviewwithemptytips.h"

class ListViewExDemoPrivate {
    QR_DECLARE_PUBLIC(ListViewExDemo)
public:
    ListViewExDemoPrivate(ListViewExDemo* q);

public:
    enum StackIndex {
        SearchResult = 0,
        DataView,
    };
};

ListViewExDemoPrivate::ListViewExDemoPrivate(ListViewExDemo *q)
    : q_ptr(q)
{}

/////////////////////////////////////////////////////////////

ListViewExDemo::ListViewExDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListViewExDemo),
    d_ptr(new ListViewExDemoPrivate(this))
{
    ui->setupUi(this);

    connect(ui->search, &QLineEdit::textChanged, [this](const QString& text){
        ui->stackedWidget->setCurrentIndex(
                    text.isEmpty() ? ListViewExDemoPrivate::StackIndex::DataView
                                   : ListViewExDemoPrivate::StackIndex::SearchResult);
    });
    ui->stackedWidget->setCurrentIndex(ListViewExDemoPrivate::StackIndex::DataView);

    auto rect = ui->dataView->friends()->listview()->rect();
    int height = ui->dataView->friends()->listview()->height();
}

ListViewExDemo::~ListViewExDemo()
{
    delete ui;
}
