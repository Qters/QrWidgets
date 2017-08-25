#ifndef FRIENDCELL_H
#define FRIENDCELL_H

#include <QWidget>

namespace Ui {
class FriendCell;
}


class FriendDelegateData;
class FriendCell : public QWidget
{
    Q_OBJECT

public:
    explicit FriendCell(QWidget *parent = 0);
    ~FriendCell();

public:
    bool init(FriendDelegateData *_data);

    int normalItemHeight() const;
    int headItemHeight() const;

private:
    Ui::FriendCell *ui;
};

#endif // FRIENDCELL_H
