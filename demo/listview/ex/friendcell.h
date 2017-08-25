#ifndef FRIENDCELL_H
#define FRIENDCELL_H

#include <QWidget>

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

public:
    bool init(FriendDelegateData *data);

    // ListviewExWidget interface
public:
    virtual int normalItemHeight() const override;
    virtual int headItemHeight() const override;

private:
    Ui::FriendCell *ui;
};

#endif // FRIENDCELL_H
