#ifndef LISTGROUPVIEW_H
#define LISTGROUPVIEW_H

#include "listview/qrlistview.h"

class ListGroupDelegate;
class ListGroupViewPrivate;
class ListGroupView : public ::Qters::QrWidgets::QrListView
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListGroupView)

public:
    ListGroupView(QWidget *parent = nullptr);
    ~ListGroupView();

public:
    void setDelegate(ListGroupDelegate *delegate);
    ListGroupDelegate* delegate();

    // QrListView interface
protected slots:
    virtual void dateChangeUpdate() override;

    // QrListView interface
protected :
    virtual QWidget *createWidget() override;

private:
    int dataChangeUpdaetImpl();
};

#endif // LISTGROUPVIEW_H
