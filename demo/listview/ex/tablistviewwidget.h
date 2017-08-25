#ifndef TABLISTVIEWWIDGET_H
#define TABLISTVIEWWIDGET_H

#include <QWidget>

#include "qrglobal.h"

namespace Ui {
class TabListviewWidget;
}

class ListviewWithEmptyTips;
class TabListviewWidgetPrivate;
class TabListviewWidget : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(TabListviewWidget)

public:
    explicit TabListviewWidget(QWidget *parent = 0);
    ~TabListviewWidget();

public:
    ListviewWithEmptyTips *friends();

private:
    Ui::TabListviewWidget *ui;
};

#endif // TABLISTVIEWWIDGET_H
