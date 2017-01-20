#include "emailinputter/qremailinputtextedit.h"

#include <QtCore/qdebug.h>
#include <QtGui/QKeyEvent>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qclipboard.h>
#include <QtWidgets/qtooltip.h>
#include <qcursor.h>

#include "emailinputter/qrmailboxblock.h"
#include "qrutf8.h"
#include "qrtoast.h"

NS_QRWIDGETS_BEGIN

class QrEMailInputTextEditPrivate{
    QR_DECLARE_PUBLIC(QrEMailInputTextEdit)

public:
    bool isShowList = false;
    QrMailboxBlockContainer meetingeeContainer;
    QTextCharFormat fmt ;

public:
    QrEMailInputTextEditPrivate(QrEMailInputTextEdit* q);
    ~QrEMailInputTextEditPrivate();

public:
    //  to left email beside
    bool onLeft();
    //  to right email beside
    bool onRight();
    //  format text to email by semicolon
    //	'name@domain.com' to 'name<name@domain.com>'
    bool onSemicolon();
    //  remove characters or email
    bool onBackspace();
    //  remove email address
    bool signalBackspace(QTextCursor *curTextCursor);
    //  remove email address's selection
    bool selectionBackspace(QTextCursor *curTextCursor);
    //	paste email addresses of clipboard
    void onPaste();
    //  paste one email address
    void pasteOneMailbox(const QString &pasteContent);
    //  highlight email if have email exist in cursor
    bool selectBlockIfExist(QTextCursor textCursor);
    //  check cursor is the last cursor of textedit
    bool isLastCursor(QTextCursor check) const;

public:
    //
    bool isValidEmail(const QString& emailAddr);

public: //  message
    void emailAdreesIsUnvalid(const QString& emailAddress);
    void emailAdreesIsExist();
    QString lastInputText(bool remove = false);
    int lastBeginPos() const;
    int lastCursorPosition() const;
};

QrEMailInputTextEditPrivate::QrEMailInputTextEditPrivate(QrEMailInputTextEdit *q)
    : q_ptr(q)
{
    q->connect(q, &QrEMailInputTextEdit::textChanged,
                     [this](){
        Q_Q(QrEMailInputTextEdit);
        emit q->startTyping(lastInputText());
    });
}

QrEMailInputTextEditPrivate::~QrEMailInputTextEditPrivate() {}

bool QrEMailInputTextEditPrivate::isLastCursor(QTextCursor check) const
{
    return check.position() == lastCursorPosition();
}

int QrEMailInputTextEditPrivate::lastCursorPosition() const
{
    Q_Q(const QrEMailInputTextEdit);
    QTextCursor lastCursor(q->document());
    lastCursor.movePosition(QTextCursor::End);
    return lastCursor.position();
}

bool QrEMailInputTextEditPrivate::onLeft()
{
    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor();

    QrMailBlock curBlock;
    if (! meetingeeContainer.previousBlock(curTextCursor.position(), &curBlock)) {
        return false;
    }

    if(curTextCursor.position() == curBlock.nextPos
            && ! curTextCursor.hasSelection()) {    //  in edit status, cursor is the last of textedit
        curTextCursor.setPosition(curBlock.endPos);
    }else if(0 == curBlock.beginPos) {
        curTextCursor.setPosition(0);
    } else {
        curTextCursor.setPosition(curBlock.beginPos - 1);
    }

    selectBlockIfExist(curTextCursor);

    return true;
}

bool QrEMailInputTextEditPrivate::onRight()
{
    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor(),
            blockTextCursor = q->textCursor();

    QrMailBlock curBlock;
    if (! meetingeeContainer.currentBlock(curTextCursor.position(), &curBlock)) {
        if(0 != meetingeeContainer.size()) {
            auto lastBlock = meetingeeContainer.lastBlock();
            if(curTextCursor.hasSelection() &&
                    lastCursorPosition() > lastBlock.nextPos) {
                auto fixCursor = q->textCursor();
                fixCursor.clearSelection();
                fixCursor.setPosition(lastBlock.nextPos);
                q->setTextCursor(fixCursor);
                return true;    //  eat event
            }
        }

        if(curTextCursor.hasSelection()) {
            QTextCursor lastCursor(q->document());
            lastCursor.movePosition(QTextCursor::End);
            q->setTextCursor(lastCursor);
        }
        return false;
    }

    if(curTextCursor.hasSelection()) {
        blockTextCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,
                                     curBlock.endPos - curTextCursor.position() + 1);
    }
    selectBlockIfExist(blockTextCursor);

    return true;
}

int QrEMailInputTextEditPrivate::lastBeginPos() const
{
    int beginPos = 0;
    if(0 != meetingeeContainer.size()) {
        beginPos = meetingeeContainer.lastBlock().endPos + 1;
    }
    return beginPos;
}

QString QrEMailInputTextEditPrivate::lastInputText(bool remove /*= false*/ )
{
    Q_Q(QrEMailInputTextEdit);

    QTextCursor curTextCursor = q->textCursor();
    auto blockSize = curTextCursor.position()-lastBeginPos();
    curTextCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, blockSize);
    curTextCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, blockSize);

    auto inputText = curTextCursor.selectedText();

    if (remove) {
        curTextCursor.removeSelectedText();
        q->setTextCursor(curTextCursor);
    }

    return inputText;
}

bool QrEMailInputTextEditPrivate::onSemicolon()
{
    Q_Q(QrEMailInputTextEdit);

    const QString selectedText = lastInputText(true);

    QrMailBlock newBlock;
    newBlock.beginPos = lastBeginPos();
    newBlock.name = selectedText.simplified();
    if ( ! isValidEmail(newBlock.name) ) {
        emailAdreesIsUnvalid(newBlock.name);

        auto fixCursor = q->textCursor();
        fixCursor.insertText(selectedText); //  fix the unvalid text
        q->setTextCursor(fixCursor);

        return false;
    }
    if (meetingeeContainer.containName(newBlock.name)) {
        emailAdreesIsExist();   //  TODO
        return false;
    }

    newBlock.display = QrMailBlock::formatDisplayByName(newBlock.name);

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

    auto curCursor = q->textCursor();
    curCursor.insertText(insertText);
    q->setTextCursor(curCursor);

    newBlock.endPos = q->textCursor().position() - 1;
    newBlock.nextPos = newBlock.endPos + 1;

    meetingeeContainer.push(newBlock);

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

    bool editLast = true;   //  edit in last area
    if(0 != meetingeeContainer.size()) {
        editLast = curTextCursor.position() > meetingeeContainer.lastBlock().endPos;
    }
    if( ! editLast) {    //  not edit in last area
        if(curTextCursor.hasSelection()) {  //   selected block
            if(! meetingeeContainer.containDisplayName(curTextCursor.selectedText())) {  //  not in container
                return false;
            }
        }
    }

    bool success = false;
    if (! curTextCursor.selectedText().isEmpty()) {
        success = selectionBackspace(&curTextCursor);
    } else {
        success = signalBackspace(&curTextCursor);
    }

    curTextCursor.removeSelectedText();
    q->setTextCursor(curTextCursor);

    return success;
}

void QrEMailInputTextEditPrivate::onPaste()
{
    QString pasteText = QApplication::clipboard()->text().simplified();
    pasteText.replace(' ', ';');
    pasteText.replace(',', ';');
    pasteText.replace('\n', ';');
    pasteText.replace('\r', ';');
    pasteText.replace(';;', ';');
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
}

bool QrEMailInputTextEditPrivate::selectBlockIfExist(QTextCursor textCursor)
{
    QrMailBlock selectedBlock;
    if ( ! meetingeeContainer.currentBlock(textCursor.position(), &selectedBlock) ) {
        return false;
    }

    qDebug() << "select block:" << selectedBlock.name;

    textCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor,
                            textCursor.position()-selectedBlock.beginPos);
    textCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                            selectedBlock.display.size());

    Q_Q(QrEMailInputTextEdit);
    q->setTextCursor(textCursor);

    return true;
}

void QrEMailInputTextEditPrivate::emailAdreesIsUnvalid(const QString &emailAddress)
{
    Q_Q(QrEMailInputTextEdit);
    QrToast::instance()->showInWidget(q, QString(QObject::tr("%1email address is unvalid.")).arg(emailAddress));
}

void QrEMailInputTextEditPrivate::emailAdreesIsExist()
{
    Q_Q(QrEMailInputTextEdit);
    QrToast::instance()->showInWidget(q, QObject::tr("email is reduplicated."));
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
    setAttribute(Qt::WA_AlwaysShowToolTips);

    setUndoRedoEnabled(false);  //  disable undo
    setContextMenuPolicy(Qt::NoContextMenu);

    auto colorToString = [&](const QColor& color){
        return QString("rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    };
    QColor highlightColor = palette().color(QPalette::Highlight);
    QColor highlightedTextColor = palette().color(QPalette::HighlightedText);
    setStyleSheet(QString("QPlainTextEdit { selection-background-color: %1; selection-color: %2; }")
                  .arg(colorToString(highlightColor), colorToString(highlightedTextColor)));
}

void QrEMailInputTextEdit::clearText()
{
    clear();
}

void QrEMailInputTextEdit::clearContainer()
{
    Q_D(QrEMailInputTextEdit);
    d->meetingeeContainer.clear();
}

void QrEMailInputTextEdit::enterClickFromDDList(const QString &ddlistValue)
{
    Q_D(QrEMailInputTextEdit);

    d->lastInputText(true);

    d->pasteOneMailbox(ddlistValue);
}

QStringList QrEMailInputTextEdit::getSimpleEmails() const
{
    Q_D(const QrEMailInputTextEdit);
    return d->meetingeeContainer.getNames();
}

QStringList QrEMailInputTextEdit::getFormatedEmails() const
{
    Q_D(const QrEMailInputTextEdit);
    return d->meetingeeContainer.getDisplayes();
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
    case Qt::Key_Return:
    case Qt::Key_Enter:
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

    bool editLast = true;   //  edit in last area
    if(0 != d->meetingeeContainer.size()) {
        editLast = textCursor().position() > d->meetingeeContainer.lastBlock().endPos;
    }
    if(textCursor().hasSelection()  //  couldn't edit in the email area
            || ! editLast ) {  //  couldn't edit except the last area
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

    if (d->isLastCursor(cursor)) {  //  could edit if in the last
        setTextCursor(cursor);
    } else {    //  only selection
        d->selectBlockIfExist(cursor);
    }
}
