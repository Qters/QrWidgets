#ifndef LISTGROUPDELEGATE_H
#define LISTGROUPDELEGATE_H

#include "listview/qrlistdelegate.h"

/*!
 * \brief The ListGroupDelegate class
 * support group view
 * sort: 1, group. 2, item in group.
 */
class ListGroupDelegatePrivate;
class ListGroupDelegate : public Qters::QrWidgets::QrListVidewDelegate
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListGroupDelegate)

public:
    ListGroupDelegate();

public:
    virtual int groupItemRenderHeight(Qters::QrWidgets::QrListViewData *data) const;

    // QrListVidewDelegate interface
public:
    virtual void sortImpl(bool update = true) override;
    virtual void addData(Qters::QrWidgets::QrListViewData *data) override;
    virtual int itemCountToShow(int listviewHeight, int itemHeight) override;
    virtual int verScrollBarRangeMaxValue(int itemHeight) override;

public:
    virtual int normalItemHeight() const = 0;
    virtual int headItemHeight() const = 0;
};

/*!
 * \brief The ListGroupData class
 * support group view's data
 */
class ListGroupDataPrivate;
class ListGroupData : public Qters::QrWidgets::QrListViewData {

    QR_DECLARE_PRIVATE(ListGroupData)

public:
    ListGroupData();

public:
    void setGroupHead(bool isHead);
    bool isGroupHead() const;

    QString groupHex() const;
    void calcGroupInfo();

public:
    virtual QString groupKey() = 0;
};

#endif // LISTGROUPDELEGATE_H
