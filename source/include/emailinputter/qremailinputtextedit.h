﻿#ifndef QRMEETINGEETEXTEDIT_H
#define QRMEETINGEETEXTEDIT_H

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
    QrEMailInputTextEdit(QWidget *parent = 0);

Q_SIGNALS:
    void startTyping(QString inputText);
    void finishTyping();

public:
    void clearText();
    void clearContainer();
    bool keyPress(QKeyEvent *event);
    void enterClickFromDDList(const QString& ddlistValue);
    //  name@domain.com
    QStringList getSimpleEmails() const;
    //  name<name@domain.com>;
    QStringList getFormatedEmails() const;

protected:
    void mousePressEvent(QMouseEvent *);
};

NS_QRWIDGETS_END

#endif // QRMEETINGEETEXTEDIT_H
