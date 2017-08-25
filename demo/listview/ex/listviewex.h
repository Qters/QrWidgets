#ifndef LISTVIEWEX_H
#define LISTVIEWEX_H

#include "listview/qrlistview.h"

class ListviewExDelegate;
class ListviewExPrivate;
class ListviewEx : public ::Qters::QrWidgets::QrListView
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListviewEx)

public:
    ListviewEx(QWidget *parent = nullptr);
    ~ListviewEx();

public:
    void setDelegate(ListviewExDelegate *delegate);
    ListviewExDelegate* delegate();

    // QrListView interface
protected slots:
    virtual void dateChangeUpdate() override;

private:
    int dataChangeUpdaetImpl();
};

#endif // LISTVIEWEX_H
