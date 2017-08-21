#ifndef LISTVIEWWIDGET_H
#define LISTVIEWWIDGET_H

#include <QWidget>

#include "qrglobal.h"

#include "listview/qrsearchlistview.h"

class ListDelegate;

class ListviewWidgetPrivate;
class ListviewWidget : public Qters::QrWidgets::QrSearchListview
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListviewWidget)

public:
    explicit ListviewWidget(QWidget *parent = 0);
    ~ListviewWidget();
};

#endif // LISTVIEWWIDGET_H
