#ifndef LISTGROUPWIDGET_H
#define LISTGROUPWIDGET_H

#include <QtWidgets/qwidget.h>

#include "qrglobal.h"

class ListGroupData;
class ListGroupWidgetPrivate;
class ListGroupWidget : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListGroupWidget)

public:
    explicit ListGroupWidget(QWidget *parent = 0);
    ~ListGroupWidget();

Q_SIGNALS:
    void onGroupHeadClick();

public:
    bool initOnlyHeadWidget(ListGroupData *data);

public:
    virtual bool initHeadWidget(ListGroupData *data);
    virtual bool initNormalWidget(ListGroupData *data);

public:
    virtual void showHeadWidget(bool show) = 0;
    virtual void showContentWidget(bool show) = 0;

    virtual int headItemHeight() const = 0;
    virtual int normalItemHeight() const = 0;
};

#endif // LISTGROUPWIDGET_H
