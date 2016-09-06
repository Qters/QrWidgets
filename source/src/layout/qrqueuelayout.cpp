#include "layout/qrqueuelayout.h"

#include <QtWidgets/qboxlayout.h>
#include <QtCore/qvector.h>
#include <QtCore/qdebug.h>

NS_QRWIDGETS_BEGIN
class QrQueueLayoutPrivate {
    QR_DECLARE_PUBLIC(QrQueueLayout)

public:
    int curRowCount = 0;
    int curColCount = 0;
    int maxRowCount = 1;
    int maxColCount = 1;

    int margin = 0;
    int spacing = 0;
    Qt::AlignmentFlag alignmentFlag = Qt::AlignCenter;

    QVector<QWidget*> widgets;
    QVBoxLayout* parentLayout;  //  layout of child layout
    QVector<QHBoxLayout*> childLayouts; //  child layout is row's layout

public:
    QrQueueLayoutPrivate(QrQueueLayout* q, QWidget *parent);

    QHBoxLayout* addChildLayout();

    bool insertWidget(const int index, QWidget *widget);
    QWidget* removeWidget(const int index);

    int index(int row, int col) const;
    bool indexValid(int index) const;
    std::pair<int, int> praseIndex(int index) const;
    bool isFull() const;
};

QrQueueLayoutPrivate::QrQueueLayoutPrivate(QrQueueLayout *q, QWidget *parent)
    : q_ptr(q)
{
    parentLayout = new QVBoxLayout;
    parentLayout->setAlignment(alignmentFlag);

    parent->setLayout(parentLayout);

    addChildLayout();
}

QHBoxLayout* QrQueueLayoutPrivate::addChildLayout()
{
    QHBoxLayout* childLayout = new QHBoxLayout;
    childLayout->setAlignment(alignmentFlag);

    parentLayout->addLayout(childLayout);
    childLayouts.push_back(childLayout);
    ++curRowCount;

    return childLayout;
}

bool QrQueueLayoutPrivate::insertWidget(const int insertIndex, QWidget *widget)
{
    if (insertIndex < 0
            || insertIndex > widgets.count()
            || isFull()) {
        return false;   //  todo index problem
    }


    auto indexRes = praseIndex(insertIndex);
    const int insertRowIndex = indexRes.first;
    const int insertColIndex = indexRes.second;

    //  check and get inserted row's layout
    QHBoxLayout* insertRowLayout = nullptr;
    if (insertRowIndex < curRowCount) {
        insertRowLayout = childLayouts.at(insertRowIndex);
        Q_ASSERT(nullptr != insertRowLayout);
    } else if(0 == insertColIndex && curRowCount != maxRowCount) {
        insertRowLayout = addChildLayout();  //  create new row layout to put widget
    } else {
        return false;
    }

    if (nullptr == insertRowLayout) {
        return false;
    }

    auto insertWidgetFunc = [this, insertRowLayout,
            insertIndex, insertColIndex, widget](){
        widgets.insert(insertIndex, widget);
        insertRowLayout->insertWidget(insertColIndex, widget);
    };

    if (insertRowLayout->count() < maxColCount) { //  inserted row's layout is not full, insert.
        insertWidgetFunc();
        return true;
    }

    //  inserted row's layout is full, insert to the last position of inserted layout

    bool fullNotInsert = false; //  kick widget if global layout is full?
    if (insertRowIndex == (maxRowCount-1)   //  inserted row is the LAST ROW of global layout
            && fullNotInsert) {    //   global layout is full.
        return false;
    }

    insertWidgetFunc();

    auto lastWidgetOfInsertRow = insertRowLayout->itemAt(insertRowLayout->count()-1)->widget();
    insertRowLayout->removeWidget(lastWidgetOfInsertRow);   //  The layout is full, remove the last widget

    //  adjust widget position cause of remove the last widget of inserted row
    QWidget* lastWidgetOfPreRow = lastWidgetOfInsertRow;

    int childRowIdx = insertRowIndex+1;
    if (childRowIdx == curRowCount  //   arrive the last row layout, need add a new layout
            && insertRowLayout->count() == maxRowCount) {
        addChildLayout();
    }

    for (; childRowIdx<curRowCount; ++childRowIdx) {
        auto nextLayout = childLayouts.at(childRowIdx);   //  next row's layout
        nextLayout->insertWidget(0, lastWidgetOfPreRow);

        if (nextLayout->count() <= maxColCount) {
            break;  //  cur row layout is not full
        }

        if (childRowIdx == (curRowCount-1)) {   //   arrive the last row layout, need add a new layout
            addChildLayout();
        }

        lastWidgetOfPreRow = nextLayout->itemAt(nextLayout->count()-1)->widget();
    }

    if (childRowIdx == maxRowCount) {   //  kick the last widget of global layout
        childLayouts.last()->removeWidget(lastWidgetOfPreRow);
    }

    return true;
}

QWidget* QrQueueLayoutPrivate::removeWidget(const int index)
{
    if (! indexValid(index)
            || widgets.isEmpty()) {
        return nullptr;
    }

    auto indexRes = praseIndex(index);
    const int removeRowIndex = indexRes.first;

    QWidget* theRemoveWidget = widgets[index];

    auto curLayout = childLayouts.at(removeRowIndex);
    curLayout->removeWidget(theRemoveWidget);
    widgets.removeOne(theRemoveWidget);
    theRemoveWidget->hide();

    if (0 == removeRowIndex
            && 1 == curRowCount) {
        return theRemoveWidget;
    }

    for(int rowIdx=removeRowIndex+1; rowIdx<curRowCount; ++rowIdx) {
        auto nextLayout = childLayouts.at(rowIdx);   //  next row's layout

        curLayout->insertWidget(curLayout->count(), nextLayout->itemAt(0)->widget());
        if (0 == nextLayout->count()) {
            childLayouts.removeLast();
            parentLayout->removeItem(nextLayout);
            delete nextLayout;
            nextLayout = nullptr;
            --curRowCount;
        }
        curLayout = nextLayout;
    }

    return theRemoveWidget;
}

bool QrQueueLayoutPrivate::indexValid(int index) const
{
    return (index >= 0) && (index < widgets.count())
            && (index < maxRowCount*maxColCount-1);
}

std::pair<int, int> QrQueueLayoutPrivate::praseIndex(int index) const
{
    if (index < 0) {
        return std::make_pair(-1, -1);
    }

    const int rowIndex = index/maxColCount;
    const int colIndex = index % maxColCount;

    return std::make_pair(rowIndex, colIndex);
}

bool QrQueueLayoutPrivate::isFull() const
{
    return widgets.count() >= maxRowCount*maxColCount;
}

int QrQueueLayoutPrivate::index(int row, int col) const
{
    int _index = row*maxRowCount + col;
    if (! indexValid(_index)) {
        return -1;
    }
    return _index;
}


NS_QRWIDGETS_END


USING_NS_QRWIDGETS;

QrQueueLayout::QrQueueLayout(QWidget *parent, int maxRowCount /*= 1*/, int maxColCount /*= 1*/)
    : d_ptr(new QrQueueLayoutPrivate(this, parent))
{
    Q_D(QrQueueLayout);
    d->maxRowCount = maxRowCount;
    d->maxColCount = maxColCount;
}

QrQueueLayout::~QrQueueLayout()
{
}

int QrQueueLayout::count() const
{
    Q_D(const QrQueueLayout);
    return d->widgets.count();
}

void QrQueueLayout::setAlignment(Qt::AlignmentFlag alignmentFlag)
{
    Q_D(QrQueueLayout);

    Q_FOREACH(QHBoxLayout* layout, d->childLayouts) {
        layout->setAlignment(alignmentFlag);
    }
    d->parentLayout->setAlignment(alignmentFlag);
    d->parentLayout->update();
    d->alignmentFlag = alignmentFlag;
}

void QrQueueLayout::setMargin(int margin)
{
    Q_D(QrQueueLayout);

    Q_FOREACH(QHBoxLayout* layout, d->childLayouts) {
        layout->setMargin(margin);
    }
    d->parentLayout->setMargin(margin);
    d->parentLayout->update();
    d->margin = margin;
}

void QrQueueLayout::setSpacing(int spacing)
{
    Q_D(QrQueueLayout);

    Q_FOREACH(QHBoxLayout* layout, d->childLayouts) {
        layout->setSpacing(spacing);
    }
    d->parentLayout->setSpacing(spacing);
    d->parentLayout->update();
    d->spacing = spacing;
}

QWidget *QrQueueLayout::widget(int row, int col) const
{
    Q_D(const QrQueueLayout);

    int widgetIndex = d->index(row, col);
    if (! d->indexValid(widgetIndex)) {
        return nullptr;
    }

    return d->widgets[widgetIndex];
}

void QrQueueLayout::pushWidget(QWidget *widget)
{
    Q_D(QrQueueLayout);

    d->insertWidget(d->widgets.count(), widget);
}

bool QrQueueLayout::insertWidget(const int row, const int col, QWidget *widget)
{
    Q_D(QrQueueLayout);

    return d->insertWidget(d->index(row, col), widget);
}

QWidget *QrQueueLayout::popWidget()
{
    Q_D(QrQueueLayout);

    return d->removeWidget(0);
}

QWidget *QrQueueLayout::removeWidget(int row, int col)
{
    Q_D(QrQueueLayout);

    return d->removeWidget(d->index(row, col));
}

bool QrQueueLayout::removeWidget(QWidget *widget)
{
    Q_D(QrQueueLayout);

    return nullptr != d->removeWidget(d->widgets.indexOf(widget));
}

int QrQueueLayout::getMaxColCount() const
{
    Q_D(const QrQueueLayout);

    return d->maxColCount;
}

void QrQueueLayout::setMaxColCount(int value)
{
    Q_D(QrQueueLayout);

    d->maxColCount = value;
}

int QrQueueLayout::getMaxRowCount() const
{
    Q_D(const QrQueueLayout);

    return d->maxRowCount;
}

void QrQueueLayout::setMaxRowCount(int value)
{
    Q_D(QrQueueLayout);

    d->maxRowCount = value;
}

int QrQueueLayout::getCurColCount() const
{
    Q_D(const QrQueueLayout);

    return d->curColCount;
}

bool QrQueueLayout::isFull() const
{
    Q_D(const QrQueueLayout);
    return d->isFull();
}

int QrQueueLayout::getCurRowCount() const
{
    Q_D(const QrQueueLayout);

    return d->curRowCount;
}
