#include "contactpage.h"
#include "ui_contactpage.h"

#include "contactdata.h"

class ContactPagePrivate{
    QR_DECLARE_PUBLIC(ContactPage)

public:
    ContactPagePrivate(ContactPage *q);

public:
    void init();
};

ContactPagePrivate::ContactPagePrivate(ContactPage *q) : q_ptr(q) {}

void ContactPagePrivate::init()
{
    Q_Q(ContactPage);
    q->connect(q->ui->filterLe, &QLineEdit::textChanged,
               [this](const QString &value){
        Q_Q(ContactPage);
        q->ui->contactList->filter(q->ui->groupCb->currentData().toInt(), value);
    });
    q->connect(q->ui->groupCb, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               [this](int index){
        Q_UNUSED(index);
        Q_Q(ContactPage);
        q->ui->contactList->filter(q->ui->groupCb->currentData().toInt(),
                                   q->ui->filterLe->text());
    });
}


ContactPage::ContactPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactPage),
    d_ptr(new ContactPagePrivate(this))
{
    ui->setupUi(this);

    Q_D(ContactPage);
    d->init();
}

ContactPage::~ContactPage()
{
    delete ui;
}

void ContactPage::init(const QVector<ContactData> &contacters)
{
    QMap<int, QString> groups;

    Q_FOREACH(ContactData contacter, contacters) {
        if (! groups.contains(contacter.groupid)) {
            groups[contacter.groupid] = contacter.groupName;
        }
    }

    ui->contactList->init(contacters);
    for(auto group : groups.toStdMap())
    {
        ui->groupCb->addItem(group.second, group.first);
    }
}
