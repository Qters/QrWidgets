#ifndef EMAILINPUTTERWIDGET_H
#define EMAILINPUTTERWIDGET_H

#include <QWidget>

namespace Ui {
class EmailInputterWidget;
}

class EmailInputterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmailInputterWidget(QWidget *parent = 0);
    ~EmailInputterWidget();

private:
    Ui::EmailInputterWidget *ui;
};

#endif // EMAILINPUTTERWIDGET_H
