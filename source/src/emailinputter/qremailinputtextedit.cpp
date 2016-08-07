#include "emailinputter/qremailinputtextedit.h"

#include <QtCore/qdebug.h>
#include <QtGui/QKeyEvent>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qclipboard.h>

#include <QtGui/qstandarditemmodel.h>

#include "emailinputter/qrmailboxblock.h"
#include "emailinputter/qrmailboxfilterproxymodel.h"

NS_QRWIDGETS_BEGIN

class QrEMailInputTextEditPrivate{
    QR_DECLARE_PUBLIC(QrEMailInputTextEdit)

public:
    bool isShowList = false;
    bool mouseClickOnEnd = false;
    int beginPosOfCurBlock = 0;
    QrMailboxBlockContainer meetingeeContainer;
    QTextCharFormat fmt ;

public:
    QString filterValue;
    QListView *listview = nullptr;
    QStandardItemModel *listdata = nullptr;
    QrMailboxFilterProxyModel *listdataProxy = nullptr;

public:
    QrEMailInputTextEditPrivate(QrEMailInputTextEdit* q);
    ~QrEMailInputTextEditPrivate();

public:
    //  set cursor on begin of the left email addreess if it exist
    bool onLeft();
    //  set cursor on begin of the right email addreess if it exist
    bool onRight();
    //  ';' to format current email address
    //	'name@domain.com' to 'name<name@domain.com>'
    bool onSemicolon();
    /*!
     * \brief remove character or email address
     * \return  event have been deal with
     */
    bool onBackspace();
    //  remove email address
    bool signalBackspace(QTextCursor *curTextCursor);
    //  remove email address's selection
    bool selectionBackspace(QTextCursor *curTextCursor);
    //  newline in inputting email address
    //  or select current selection of email-list
    bool onEnter();
    //	paste email addresses of clipboard
    void onPaste();
    //  paste one email address
    void pasteOneMailbox(const QString &pasteContent);
    //  select the above selection of email-list
    void onUp();
    //  select the below selection of email-list
    void onDown();

public: //  drop-down list
    //  create
    bool createDDListView();
    //
    void updateBeginPosOfCurBlock(int position);
    //
    bool isValidEmail(const QString& emailAddr);
    //  move selection of list
    void moveListView(bool up);

public: //  message
    void emailAdreesIsUnvalid(const QString& emailAddress);
    void emailAdreesIsExist();
};

QrEMailInputTextEditPrivate::QrEMailInputTextEditPrivate(QrEMailInputTextEdit *q)
    : q_ptr(q) {}

QrEMailInputTextEditPrivate::~QrEMailInputTextEditPrivate(){
}

void QrEMailInputTextEditPrivate::updateBeginPosOfCurBlock(int position){
    beginPosOfCurBlock = position;
}

bool QrEMailInputTextEditPrivate::onLeft()
{
    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor();

    QrMailBlock preBlock;
    if (! meetingeeContainer.previousBlock(curTextCursor.position(), &preBlock)) {
        return false;
    }

    curTextCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor,
                               curTextCursor.position() - preBlock.beginPos);
    q->setTextCursor(curTextCursor);

    return true;
}

bool QrEMailInputTextEditPrivate::onRight()
{
    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor();

    QrMailBlock curBlock;
    if (! meetingeeContainer.currentBlock(curTextCursor.position(), &curBlock)) {
        return false;
    }

    curTextCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,
                               curBlock.endPos - curTextCursor.position() + 1);
    q->setTextCursor(curTextCursor);

    return true;
}

bool QrEMailInputTextEditPrivate::onSemicolon()
{
    Q_Q(QrEMailInputTextEdit);

    QrMailBlock newBlock;
    newBlock.beginPos = beginPosOfCurBlock;

    QTextCursor curTextCursor = q->textCursor();
    auto blockSize = curTextCursor.position()-beginPosOfCurBlock;
    curTextCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, blockSize);
    curTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, blockSize);

    const QString selectedText = curTextCursor.selectedText();

    newBlock.name = selectedText.simplified();
    if ( ! isValidEmail(newBlock.name) ) {
        emailAdreesIsUnvalid(newBlock.name);
        return false;
    }
    if (meetingeeContainer.containName(newBlock.name)) {
        emailAdreesIsExist();
        return false;
    }

    newBlock.display = QrMailBlock::formatDisplayByName(newBlock.name);

    curTextCursor.removeSelectedText();
    QString insertText = newBlock.display;

    //  \n \r
    QString leftNewLineValue = selectedText.mid(0,1);
    QString rightNewLineValue = selectedText.right(1);
    if (leftNewLineValue.simplified() != leftNewLineValue) {
        insertText = leftNewLineValue + insertText;
    }
    if (rightNewLineValue.simplified() != rightNewLineValue) {
        insertText = insertText + rightNewLineValue;
    }

    curTextCursor.insertText(insertText);
    q->setTextCursor(curTextCursor);

    newBlock.endPos = q->textCursor().position() - 1;
    newBlock.nextPos = newBlock.endPos + 1;

    meetingeeContainer.push(newBlock);

    updateBeginPosOfCurBlock(newBlock.nextPos);  //  begin position of next block

    filterValue.clear();    //  one mailbox finish, clear filterValue

    return true;
}

bool QrEMailInputTextEditPrivate::selectionBackspace(QTextCursor *curTextCursor)
{
    const QString split = ";";
    QStringList selectedBlocks = curTextCursor->selectedText().split(split);

    Q_FOREACH(QString selectBlock, selectedBlocks) {
        if (selectBlock.isEmpty()) {
            continue;
        }
        meetingeeContainer.removeByDisplay(selectBlock + split);
    }

    return true;
}

bool QrEMailInputTextEditPrivate::onEnter()
{
    auto selectIndex = listview->selectionModel()->currentIndex();
    if (! selectIndex.isValid()) {
        return false;
    }

    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor();

    auto removeSize = curTextCursor.position()-beginPosOfCurBlock;
    curTextCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, removeSize);
    curTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, removeSize);

    curTextCursor.removeSelectedText();

    q->setTextCursor(curTextCursor);

    pasteOneMailbox(listdataProxy->data(selectIndex).toString());

    return true;
}

bool QrEMailInputTextEditPrivate::signalBackspace(QTextCursor *curTextCursor)
{
    QrMailBlock deletingBlock;
    if (! meetingeeContainer.previousBlock(curTextCursor->position(), &deletingBlock)) {
        return false;
    }

    auto blockSize = deletingBlock.display.size();
    curTextCursor->movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, blockSize);
    curTextCursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, blockSize);

    meetingeeContainer.removeByDisplay(curTextCursor->selectedText());

    return true;
}

bool QrEMailInputTextEditPrivate::onBackspace()
{
    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor();

    bool success = false;
    if (! curTextCursor.selectedText().isEmpty()) {
        success = selectionBackspace(&curTextCursor);
    } else {
        success = signalBackspace(&curTextCursor);
    }

    curTextCursor.removeSelectedText();
    q->setTextCursor(curTextCursor);
    if (success) {
        updateBeginPosOfCurBlock(curTextCursor.position());
    }

    return success;
}

void QrEMailInputTextEditPrivate::onPaste()
{
    const QString pasteText = QApplication::clipboard()->text().simplified();
    const QStringList tempPasteBlockContents = pasteText.split(';');

    //  simplifize
    QStringList pasteBlockContents;
    Q_FOREACH(QString tempValue, tempPasteBlockContents) {
        tempValue = tempValue.simplified();
        if (tempValue.isEmpty()) {
            continue;
        }
        pasteBlockContents.push_back(tempValue);
    }

    //  check
    Q_FOREACH(QString pasteBlockContent, pasteBlockContents) {
        if (! isValidEmail(pasteBlockContent)) {
            emailAdreesIsUnvalid(pasteBlockContent);
            return ;
        }
    }

    //  insert
    Q_FOREACH(QString pasteBlockContent, pasteBlockContents) {
        pasteOneMailbox(pasteBlockContent);
    }
}

void QrEMailInputTextEditPrivate::pasteOneMailbox(const QString& pasteContent)
{
    Q_Q(QrEMailInputTextEdit);

    if (meetingeeContainer.containName(pasteContent)) {
        emailAdreesIsExist();
        return;
    }


    QTextCursor curTextCursor = q->textCursor();
    int curPosition = curTextCursor.position();

    QrMailBlock newBlock;
    newBlock.beginPos = curPosition;
    newBlock.name = pasteContent;
    newBlock.display = QrMailBlock::formatDisplayByName(newBlock.name);
    newBlock.endPos = curPosition + newBlock.display.size() - 1;
    newBlock.nextPos = curPosition = newBlock.endPos + 1;

    meetingeeContainer.push(newBlock);

    curTextCursor.insertText(newBlock.display);
    q->setTextCursor(curTextCursor);

    updateBeginPosOfCurBlock(curTextCursor.position());
}

void QrEMailInputTextEditPrivate::moveListView(bool up)
{
    if (nullptr == listdata
            || 0 == listdataProxy->rowCount()
            || filterValue.isEmpty()) {
        return;
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
}

void QrEMailInputTextEditPrivate::emailAdreesIsUnvalid(const QString &emailAddress)
{
    Q_Q(QrEMailInputTextEdit);
    QMessageBox::information(q,
                             QObject::tr("warning"),
                             QObject::tr(QString("[%1]email address is unvalid.")
                                         .arg(emailAddress).toStdString().c_str()));
}

void QrEMailInputTextEditPrivate::emailAdreesIsExist()
{
    Q_Q(QrEMailInputTextEdit);
    QMessageBox::information(q,
                             QObject::tr("warning"),
                             QObject::tr(QString("email address is exist.").toStdString().c_str()));
}

void QrEMailInputTextEditPrivate::onUp()
{
    moveListView(true);
}

void QrEMailInputTextEditPrivate::onDown()
{
    moveListView(false);
}

bool QrEMailInputTextEditPrivate::createDDListView() {
    Q_Q(QrEMailInputTextEdit);

    if (nullptr != listview) {
        return false;
    }

    listview = new QListView(q->parentWidget());

    QObject::connect(q, &QrEMailInputTextEdit::textChanged,
                     q, &QrEMailInputTextEdit::startTyping);
    QObject::connect(q, &QrEMailInputTextEdit::startTyping, [this](){
        Q_Q(QrEMailInputTextEdit);

        QTextCursor filterValueCursor = q->textCursor();
        int filterValueWidth = filterValueCursor.position() - beginPosOfCurBlock;
        filterValueCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor,
                                       filterValueWidth);
        filterValueCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                                       filterValueWidth);
        filterValue = filterValueCursor.selectedText();

        if (filterValue.isEmpty()) {
            listview->hide();
            return;
        }

        auto qPos = q->mapToGlobal(q->pos());
        qPos.setY(qPos.y() + q->height());
        listview->move(qPos);

        listdataProxy->setFilterRegExp(filterValue);
        listview->show();
    });
    QObject::connect(q, &QrEMailInputTextEdit::finishTyping,
                     listview, &QListView::hide);


    listview->setWindowFlags(Qt::CustomizeWindowHint
                             | Qt::FramelessWindowHint
                             | Qt::Tool);
    listview->resize(q->width(), q->height());

    listdataProxy = new QrMailboxFilterProxyModel(listview);
    Q_ASSERT(nullptr != listdata);
    listdataProxy->setSourceModel(listdata);
    listdataProxy->setFilterKeyColumn(0);
    listdataProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    listview->setModel(listdataProxy);
    listview->setFocusPolicy(Qt::NoFocus);
    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    return true;
}

bool QrEMailInputTextEditPrivate::isValidEmail(const QString &emailAddr)
{
    QString emailRegExp = "\\b([A-Z0-9._%+-]+)@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b";
    QRegExp emailReg(emailRegExp);
    emailReg.setCaseSensitivity(Qt::CaseInsensitive);
    emailReg.setPatternSyntax(QRegExp::RegExp);

    return emailReg.exactMatch(emailAddr);
}

NS_QRWIDGETS_END

////////////////////////////////////////////////////////////////

USING_NS_QRWIDGETS;

QrEMailInputTextEdit::QrEMailInputTextEdit(QWidget *parent)
    : QTextEdit(parent),
      d_ptr(new QrEMailInputTextEditPrivate(this))
{
    parent->installEventFilter(this);

    d_ptr->updateBeginPosOfCurBlock(textCursor().position());

    setUndoRedoEnabled(false);  //  disable undo

    auto colorToString = [&](const QColor& color){
        return QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    };
    QColor highlightColor = palette().color(QPalette::Highlight);
    QColor highlightedTextColor = palette().color(QPalette::HighlightedText);
    setStyleSheet(QString("QPlainTextEdit { selection-background-color: %1; selection-color: %2; }")
                  .arg(colorToString(highlightColor), colorToString(highlightedTextColor)));
}

bool QrEMailInputTextEdit::showDropDownList(QStandardItemModel *listData)
{
    if (nullptr == listData) {
        return false;
    }

    Q_D(QrEMailInputTextEdit);

    d->listdata = listData;
    return d->createDDListView();
}

void QrEMailInputTextEdit::setHeightOfDDList(int height)
{
    Q_D(QrEMailInputTextEdit);
    d->listview->resize(d->listview->width(), height);
}

void QrEMailInputTextEdit::keyPressEvent(QKeyEvent *event)
{
    Q_D(QrEMailInputTextEdit);

    switch(event->key()){
    case Qt::Key_Left:
        if (d->onLeft()) {
            return;
        }
        break;
    case Qt::Key_Right:
        if (d->onRight()) {
            return;
        }
        break;
    case Qt::Key_Up:
        d->onUp();
        return;
    case Qt::Key_Down:
        d->onDown();
        return;
    case Qt::Key_Semicolon:
        d->onSemicolon();
        return;
    case Qt::Key_Backspace:
        if( d->onBackspace()){
            return;
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if( d->onEnter()){
            return;
        }
        break;
        break;
    }

    if (event->matches(QKeySequence::Paste)) {
        d->onPaste();
        return;
    } else if(event->matches(QKeySequence::Delete)
              || event->matches(QKeySequence::Undo)
              || event->matches(QKeySequence::Redo)) {
        return;
    }

    QTextEdit::keyPressEvent(event);
}

void QrEMailInputTextEdit::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent";
    Q_D(QrEMailInputTextEdit);

    QTextCursor cursor = cursorForPosition(event->pos());

    QTextCursor lastCursor(document());
    lastCursor.movePosition(QTextCursor::End);
    d->mouseClickOnEnd = (cursor.position() == lastCursor.position());

    QrMailBlock selectedBlock;
    if ( ! d->meetingeeContainer.currentBlock(cursor.position(), &selectedBlock) ) {
        return QTextEdit::mousePressEvent(event);
    }

    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, cursor.position()-selectedBlock.beginPos);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, selectedBlock.display.size());

    setTextCursor(cursor);
}

void QrEMailInputTextEdit::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QrEMailInputTextEdit);

    if (! d->mouseClickOnEnd) {
        return;
    }

    QTextCursor cursor = cursorForPosition(event->pos());

    QTextCursor lastCursor(document());
    lastCursor.movePosition(QTextCursor::End);

    if (cursor.position() >= lastCursor.position()) {
        cursor.clearSelection();
        setTextCursor(cursor);
        return;
    }

    QrMailBlock selectedBlock;
    if ( ! d->meetingeeContainer.currentBlock(cursor.position(), &selectedBlock) ) {
        return QTextEdit::mouseMoveEvent(event);
    }

    if ( ( cursor.position() - (selectedBlock.beginPos + selectedBlock.endPos) / 2) < 2 ) {
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, cursor.position()-selectedBlock.beginPos);
        cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor, selectedBlock.display.size()+1);

        setTextCursor(cursor);
    }
}

bool QrEMailInputTextEdit::eventFilter(QObject *target, QEvent *event)
{
    qDebug() << event;
    if (parent() == target
            && event->type() == QEvent::Move) {
        QMoveEvent *moveEvent = static_cast<QMoveEvent*>(event);
        qDebug() << "parent moving..  " << moveEvent->pos();
    }

    return QTextEdit::eventFilter(target, event);
}
