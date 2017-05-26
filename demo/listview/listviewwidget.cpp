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
        ui->listview->delegate()->filter([filterReg](QrListViewData* _data) -> bool{
            ListCellData* data = static_cast<ListCellData*>(_data);
            if(nullptr == data) {
                return false;
            }
            return filterReg.exactMatch(data->username);
        });
    });
}

ListviewWidget::~ListviewWidget()
{
    delete ui;
}
