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
    virtual bool initHeadPartWidget(ListGroupData *data) override;
    virtual bool initContentPartWidget(ListGroupData *data) override;

    virtual void showHeadWidget(bool show) override;
    virtual void showContentWidget(bool show) override;

    virtual int entireHeight() const override;
    virtual int contentHeight() const override;

private:
    Ui::FriendCell *ui;
};

#endif // FRIENDCELL_H
