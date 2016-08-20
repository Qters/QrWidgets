#ifndef QREDITABLELABEL_H
#define QREDITABLELABEL_H

#include <QtWidgets/qwidget.h>
#include <QtCore/qregexp.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

class QrEditableLabelPrivate;

class QRWIDGETSSHARED_EXPORT QrEditableLabel : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrEditableLabel)

public:
    QrEditableLabel(QWidget* parent = Q_NULLPTR, bool editable = true);

Q_SIGNALS:
    void textChanged(const QString& value);

public:
    void createLayout();
    void setEditable(bool editable);

    void setTextRegex(const QRegExp& regexp);
    void setText(const QString& text);
    QString text() const;

    // QObject interface
public:
    virtual bool eventFilter(QObject *, QEvent *) override;
};

NS_QRWIDGETS_END

#endif // QREDITABLELABEL_H
