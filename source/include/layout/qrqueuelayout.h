#ifndef QRQUEUELAYOUT_H
#define QRQUEUELAYOUT_H

#include <QtWidgets/qwidget.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

/*!
 * \brief show like gird, sort like queue
 */

class QrQueueLayoutPrivate;

class QRWIDGETSSHARED_EXPORT QrQueueLayout
{
    QR_DECLARE_PRIVATE(QrQueueLayout)

public:
    QrQueueLayout(QWidget *parent, int maxRowCount = 1, int maxColCount = 1);
    virtual ~QrQueueLayout();

public:
    int count() const;
    bool isFull() const;
    void setAlignment(Qt::AlignmentFlag alignmentFlag);
    void setMargin(int margin);
    void setSpacing(int spacing);

public:
    QWidget *widget(int row, int col) const;

    void pushWidget(QWidget *widget);
    bool insertWidget(const int row, const int col, QWidget *widget);

    QWidget *popWidget();
    QWidget *removeWidget(int row, int col);
    bool removeWidget(QWidget *widget);

public:
    int getMaxColCount() const;
    void setMaxColCount(int value);

    int getMaxRowCount() const;
    void setMaxRowCount(int value);

    int getCurRowCount() const;
    int getCurColCount() const;
};

NS_QRWIDGETS_END

#endif // QRQUEUELAYOUT_H
