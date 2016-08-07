#ifndef MEETINGEETEXTEDIT_H
#define MEETINGEETEXTEDIT_H

#include <QtWidgets/qtextedit.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

NS_QRWIDGETS_BEGIN

class QrEMailInputTextEditPrivate;

/*!
 * \brief email inputter
 * take one email address as one block
 *
 * 1.   privide contacts drop-down list, and use up or down to select, enter to input
 * 2.   input emicolon to format email address,
 *      'name@domail.com' to 'name<name@email.com>',
 *      and support paste email-address from clipboard.
 * 3.   use left or right select previous or behind one.
 *
 */
class QRWIDGETSSHARED_EXPORT QrEMailInputTextEdit : public QTextEdit
{
    Q_OBJECT
    QR_DECLARE_PRIVATE(QrEMailInputTextEdit)

public:
    QrEMailInputTextEdit(QWidget *parent);

Q_SIGNALS:
    void startTyping();
    void finishTyping();

public:
    //  use contacts drop-down list
    bool showDropDownList(QStandardItemModel *listData);
    //  set height of drop-down list
    void setHeightOfDDList(int height);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    // QObject interface
public:
    virtual bool eventFilter(QObject *, QEvent *) override;
};

NS_QRWIDGETS_END

#endif // MEETINGEETEXTEDIT_H
