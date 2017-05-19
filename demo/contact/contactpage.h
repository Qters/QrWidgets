#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

#include <QWidget>

#include "qrglobal.h"

namespace Ui {
class ContactPage;
}

class ContactData;

class ContactPagePrivate;
class ContactPage : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ContactPage)

public:
    explicit ContactPage(QWidget *parent = 0);
    ~ContactPage();

public:
    void init(const QVector<ContactData> &contacters);

private:
    Ui::ContactPage *ui;
};

#endif // CONTACTPAGE_H
