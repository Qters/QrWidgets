#include "listviewwidget.h"
#include "ui_listviewwidget.h"

#include "listdelegate.h"

USING_NS_QRWIDGETS;

ListviewWidget::ListviewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListviewWidget)
{
    ui->setupUi(this);

    delegate = new ListDelegate();
    for(int i=0; i<100000; i++) {
        auto username = QString("User %1").arg(i);
        delegate->appendUser(username,
                             QString("Sign of %1.").arg(username));
    }

    ui->listview->setDelegate(delegate);

    connect(ui->filter, &QLineEdit::textChanged, [this](const QString &text){
        QRegExp filterReg(QString("*%1*").arg(text));
        filterReg.setCaseSensitivity(Qt::CaseInsensitive);
        filterReg.setPatternSyntax(QRegExp::Wildcard);
        ui->listview->delegate()->filter(filterReg);
    });
}

ListviewWidget::~ListviewWidget()
{
    delete ui;
}
