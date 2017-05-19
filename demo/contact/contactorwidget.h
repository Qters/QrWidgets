#ifndef CONTACTOR_H
#define CONTACTOR_H

#include <QWidget>

namespace Ui {
class ContactorWidget;
}

class ContactorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContactorWidget(QWidget *parent = 0);
    ~ContactorWidget();

public:
    void init(int level, const QString& name);

private:
    Ui::ContactorWidget *ui;
    static int count;
};

#endif // CONTACTOR_H
