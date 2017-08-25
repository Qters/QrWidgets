#ifndef GROUPCELL_H
#define GROUPCELL_H

#include "listgroupwidget.h"

namespace Ui {
class GroupCell;
}

class GroupCell : public ListGroupWidget
{
    Q_OBJECT

public:
    explicit GroupCell(QWidget *parent = 0);
    ~GroupCell();

    // ListGroupWidget interface
public:
    virtual bool initHeadWidget(ListGroupData *data) override;
    virtual bool initOnlyHeadWidget(ListGroupData *data) override;
    virtual bool initNormalWidget(ListGroupData *data) override;

    virtual int headItemHeight() const override;
    virtual int normalItemHeight() const override;

private:
    Ui::GroupCell *ui;
};

#endif // GROUPCELL_H
