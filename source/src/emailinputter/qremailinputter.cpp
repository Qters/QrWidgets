#include "emailinputter/qremailinputter.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qtextedit.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qlistview.h>
#include <QtGui/qstandarditemmodel.h>
#include <QtWidgets/qboxlayout.h>

#include "emailinputter/qrmailboxfilterproxymodel.h"
#include "emailinputter/qremailinputtextedit.h"

NS_QRWIDGETS_BEGIN

class QrEmailInputterPrivate{
    QR_DECLARE_PUBLIC(QrEmailInputter)

public:
    QrEMailInputTextEdit *textedit = nullptr;

    QString filteredValue;
    QListView *listview = nullptr;
    QStandardItemModel *listdata = nullptr;
    QrMailboxFilterProxyModel *listdataProxy = nullptr;

public:
    QrEmailInputterPrivate(QrEmailInputter *q) : q_ptr(q) {}

public:
    void initLayout();
    void connectListView();
    bool upDownListView(bool up);
    void showListView();
};

void QrEmailInputterPrivate::initLayout() {
    Q_Q(QrEmailInputter);
    textedit = new QrEMailInputTextEdit(q);
    textedit->installEventFilter(q);

    listview = new QListView(q);
    listview->installEventFilter(q);
    listview->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::Tool );

    listdataProxy = new QrMailboxFilterProxyModel(listview);
    listdataProxy->setFilterKeyColumn(0);
    listdataProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    listview->setModel(listdataProxy);
    listview->setFocusPolicy(Qt::NoFocus);
    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listview->hide();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(textedit);
    layout->addWidget(listview);

    q->setLayout(layout);
}

void QrEmailInputterPrivate::connectListView() {
    QObject::connect(textedit, &QrEMailInputTextEdit::startTyping,
                     [this](int beginPositionOfBlock){
        QTextCursor filterValueCursor = textedit->textCursor();
        int filterValueWidth = filterValueCursor.position() - beginPositionOfBlock;
        filterValueCursor.movePosition(
                    QTextCursor::Left,
                    QTextCursor::MoveAnchor,
                    filterValueWidth);
        filterValueCursor.movePosition(
                    QTextCursor::Right,
                    QTextCursor::KeepAnchor,
                    filterValueWidth);
        filteredValue = filterValueCursor.selectedText();

        if (filteredValue.isEmpty()) {
            listview->hide();
            return;
        }
        listdataProxy->setFilterRegExp(filteredValue);

        if (0 == listdataProxy->rowCount()) {
            listview->hide();
            return;
        }
        showListView();
    });
    QObject::connect(textedit, &QrEMailInputTextEdit::finishTyping, [this](){
        filteredValue.clear();    //  one mailbox finish, clear filterValue

        listview->hide();
    });
}

bool QrEmailInputterPrivate::upDownListView(bool up)
{
    if (nullptr == listdata
            || 0 == listdataProxy->rowCount()
            || filteredValue.isEmpty()) {
        return false;
    }

    int curRowIdx = listview->selectionModel()->currentIndex().row();
    if (-1 == curRowIdx) {
        curRowIdx = 0;
    }
    int nextRowIdx = (up ? --curRowIdx : ++ curRowIdx) % listdataProxy->rowCount();
    if (up && -1 == nextRowIdx) {
        nextRowIdx = listdataProxy->rowCount() - 1;
    }
    listview->selectionModel()->setCurrentIndex(
                listdataProxy->index(nextRowIdx, 0), QItemSelectionModel::SelectCurrent);

    return true;
}

void QrEmailInputterPrivate::showListView()
{
    listview->resize(textedit->size());

    auto qPos = textedit->mapToGlobal(textedit->pos());
    qPos.setY(qPos.y() + textedit->height());
    listview->move(qPos - QPoint(10,10));

    listview->show();
}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

QrEmailInputter::QrEmailInputter(QWidget *parent)
    : QWidget(parent),
      d_ptr(new QrEmailInputterPrivate(this))
{
    Q_D(QrEmailInputter);
    d->initLayout();
    d->connectListView();
}

void QrEmailInputter::setDDListData(QStandardItemModel *listData)
{
    Q_ASSERT(nullptr != listData);

    Q_D(QrEmailInputter);
    d->listdata = listData;
    d->listdataProxy->setSourceModel(d->listdata);
}

void QrEmailInputter::setHeightOfDDList(int height)
{
    Q_D(QrEmailInputter);
    d->listview->resize(d->listview->width(), height);
}

bool QrEmailInputter::keyPress(QKeyEvent *event)
{
    Q_D(QrEmailInputter);

    switch(event->key()){
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (! d->listview->selectionModel()->currentIndex().isValid()) {
            return d->textedit->keyPress(event);
        }
        d->textedit->enterClickFromDDList(
                    d->listdataProxy->data(
                        d->listview->selectionModel()->currentIndex())
                    .toString());
        return true;
    case Qt::Key_Up:    //  select the above selection of email-list
        return d->upDownListView(true);
    case Qt::Key_Down:  //  select the below selection of email-list
        return d->upDownListView(false);
    }

    return d->textedit->keyPress(event);
}

bool QrEmailInputter::eventFilter(QObject *target, QEvent *event)
{
    Q_D(QrEmailInputter);
    if (d->textedit == target
            && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyPress(keyEvent)) {
            return true;
        }
    }

    return QWidget::eventFilter(target, event);
}

void QrEmailInputter::hideEvent(QHideEvent *)
{
    Q_D(QrEmailInputter);

    d->textedit->hide();
    d->listview->hide();
}
