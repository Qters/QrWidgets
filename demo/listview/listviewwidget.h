#ifndef LISTVIEWWIDGET_H
#define LISTVIEWWIDGET_H

#include <QWidget>

#include "qrglobal.h"

class ListDelegate;

namespace Ui {
class ListviewWidget;
}

class ListviewWidgetPrivate;
class ListviewWidget : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListviewWidget)

public:
    explicit ListviewWidget(QWidget *parent = 0);
    ~ListviewWidget();

private:
    Ui::ListviewWidget *ui;
};

#endif // LISTVIEWWIDGET_H
