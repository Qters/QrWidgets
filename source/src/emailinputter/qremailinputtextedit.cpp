#include "emailinputter/qremailinputtextedit.h"

#include <QtCore/qdebug.h>
#include <QtGui/QKeyEvent>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qclipboard.h>

#include "emailinputter/qrmailboxblock.h"

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
    //	paste email addresses of clipboard
    void onPaste();
    //  paste one email address
    void pasteOneMailbox(const QString &pasteContent);

public:
    //
    void updateBeginPosOfCurBlock(int position);
    //
    bool isValidEmail(const QString& emailAddr);

public: //  message
    void emailAdreesIsUnvalid(const QString& emailAddress);
    void emailAdreesIsExist();
};

QrEMailInputTextEditPrivate::QrEMailInputTextEditPrivate(QrEMailInputTextEdit *q)
    : q_ptr(q)
{
    QObject::connect(q, &QrEMailInputTextEdit::textChanged,
                     [this](){
        Q_Q(QrEMailInputTextEdit);
        emit q->startTyping(beginPosOfCurBlock);
    });
}

QrEMailInputTextEditPrivate::~QrEMailInputTextEditPrivate() {}

void QrEMailInputTextEditPrivate::updateBeginPosOfCurBlock(int position)
{
    beginPosOfCurBlock = position;

    Q_Q(QrEMailInputTextEdit);
    emit q->finishTyping();
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

    updateBeginPosOfCurBlock(curTextCursor.position());
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

    updateBeginPosOfCurBlock(curTextCursor.position());

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

void QrEMailInputTextEditPrivate::emailAdreesIsUnvalid(const QString &emailAddress)
{
    Q_Q(QrEMailInputTextEdit);
    QMessageBox::information(q,
                             QObject::tr("warning"),
                             QObject::tr(QString("[%1] email address is unvalid.")
                                         .arg(emailAddress).toStdString().c_str()));
}

void QrEMailInputTextEditPrivate::emailAdreesIsExist()
{
    Q_Q(QrEMailInputTextEdit);
    QMessageBox::information(q,
                             QObject::tr("warning"),
                             QObject::tr(QString("email address is exist.").toStdString().c_str()));
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

void QrEMailInputTextEdit::enterClickFromDDList(const QString &ddlistValue)
{
    Q_D(QrEMailInputTextEdit);

    QTextCursor curTextCursor = textCursor();

    auto removeSize = curTextCursor.position()-d->beginPosOfCurBlock;
    curTextCursor.movePosition(QTextCursor::Left,
                               QTextCursor::MoveAnchor,
                               removeSize);
    curTextCursor.movePosition(QTextCursor::Right,
                               QTextCursor::KeepAnchor,
                               removeSize);
    curTextCursor.removeSelectedText();

    setTextCursor(curTextCursor);

    d->pasteOneMailbox(ddlistValue);
}

bool QrEMailInputTextEdit::keyPress(QKeyEvent *event)
{
    Q_D(QrEMailInputTextEdit);

    bool deleteKeyPress = false;
    bool insertKeyPress = false;

    switch(event->key()){
    case Qt::Key_Left:
        if (d->onLeft()) {
            return true;
        }
        break;
    case Qt::Key_Right:
        if (d->onRight()) {
            return true;
        }
        break;
    case Qt::Key_Semicolon:
        insertKeyPress = true;
        d->onSemicolon();
        return true;
    case Qt::Key_Backspace:
        deleteKeyPress = true;
        if( d->onBackspace()){
            return true;
        }
        break;
    default:
        insertKeyPress = true;
        break;
    }

    if (event->matches(QKeySequence::Paste)) {
        insertKeyPress = true;
        d->onPaste();
        return true;
    } else if(event->matches(QKeySequence::Delete)
              || event->matches(QKeySequence::Undo)
              || event->matches(QKeySequence::Redo)) {
        return true;
    }

    auto curPosition = textCursor().position();
    if ( d->meetingeeContainer.size() > 0    //  container have elements
         && d->meetingeeContainer.lastBlock().endPos >= curPosition ) {	//	not change in the end block
        if (deleteKeyPress) {    //  delete a character
            d->meetingeeContainer.updatePosWhenDeleteOneKey(curPosition);
        } else if (insertKeyPress) {    //  input a character
            d->meetingeeContainer.updatePosWhenInputOneKey(curPosition);
        }
    }

    return false;
}

void QrEMailInputTextEdit::mousePressEvent(QMouseEvent *event)
{
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
