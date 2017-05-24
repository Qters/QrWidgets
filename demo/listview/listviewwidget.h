#ifndef LISTVIEWWIDGET_H
#define LISTVIEWWIDGET_H

#include <QWidget>

class ListDelegate;

namespace Ui {
class ListviewWidget;
}

class ListviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListviewWidget(QWidget *parent = 0);
    ~ListviewWidget();

private:
    Ui::ListviewWidget *ui;

    ListDelegate* delegate = nullptr;
};

#endif // LISTVIEWWIDGET_H
