﻿#ifndef QREMAILINPUTTER_H
#define QREMAILINPUTTER_H

#include <QtWidgets/qwidget.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

NS_QRWIDGETS_BEGIN

class QrEmailInputterPrivate;
class QrEMailInputTextEdit;
class QRWIDGETSSHARED_EXPORT QrEmailInputter : public QWidget
{
//    Q_OBJECT
    QR_DECLARE_PRIVATE(QrEmailInputter)

public:
    QrEmailInputter(QWidget *parent = 0);
    virtual ~QrEmailInputter() = default;

public:
    //  set email drop-down list's data
    void setDDListData(QStandardItemModel *listData);
    //  set height of drop-down list
    void setHeightOfDDList(int height);
    //
    void setMoveBy(QWidget *widget);
    //
    QString text() const;
    //
    void clear();
    //
    QrEMailInputTextEdit* getEMailInputTextEdit()const;
    //
    void fillSemicolon();
    //
    bool isContentValid() const;
    //
    QStringList getSimpleEmails() const;
    //
    QStringList getEmails() const;

private:
    bool keyPress(QKeyEvent *event) ;

    // QObject interface
public:
    virtual bool eventFilter(QObject *, QEvent *) override;
};

NS_QRWIDGETS_END

#endif // QREMAILINPUTTER_H
