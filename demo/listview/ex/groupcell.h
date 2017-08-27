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
    virtual bool initHeadPartWidget(ListGroupData *data) override;
    virtual bool initContentPartWidget(ListGroupData *data) override;

    virtual void showHeadWidget(bool show) override;
    virtual void showContentWidget(bool show) override;

    virtual int entireHeight() const override;
    virtual int contentHeight() const override;

private:
    Ui::GroupCell *ui;
};

#endif // GROUPCELL_H
