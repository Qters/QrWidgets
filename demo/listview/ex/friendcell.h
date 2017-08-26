#ifndef FRIENDCELL_H
#define FRIENDCELL_H

#include "listgroupwidget.h"

namespace Ui {
class FriendCell;
}

class FriendDelegateData;
class FriendCell : public ListGroupWidget
{
    Q_OBJECT

public:
    explicit FriendCell(QWidget *parent = 0);
    ~FriendCell();

    // ListviewExWidget interface
public:
    virtual bool initHeadWidget(ListGroupData *data) override;
    virtual bool initNormalWidget(ListGroupData *data) override;

    virtual void showHeadWidget(bool show) override;
    virtual void showContentWidget(bool show) override;

    virtual int headItemHeight() const override;
    virtual int normalItemHeight() const override;

private:
    Ui::FriendCell *ui;
};

#endif // FRIENDCELL_H
