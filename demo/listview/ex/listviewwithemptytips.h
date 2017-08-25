#ifndef LISTVIEWWITHEMPTYTIPS_H
#define LISTVIEWWITHEMPTYTIPS_H

#include <QWidget>

#include "qrglobal.h"
#include "listview/qrlistview.h"

//class Qters::QrWidgets::QrListView;

namespace Ui {
class ListviewWithEmptyTips;
}

class ListviewWithEmptyTipsPrivate;
class ListviewWithEmptyTips : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListviewWithEmptyTips)

public:
    explicit ListviewWithEmptyTips(QWidget *parent = 0);
    ~ListviewWithEmptyTips();

public:
    void setDataEmptyTips(const QString &tips);
    Qters::QrWidgets::QrListView *listview();

private:
    Ui::ListviewWithEmptyTips *ui;
};

#endif // LISTVIEWWITHEMPTYTIPS_H
