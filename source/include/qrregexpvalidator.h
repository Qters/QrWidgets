#ifndef QRREGEXPVALIDATOR_H
#define QRREGEXPVALIDATOR_H

#include <QtGui/qvalidator.h>

#include "qrglobal.h"

#include "qrwidgets_global.h"

NS_QRWIDGETS_BEGIN

/*!
 * \brief RegExpValidator with tooltips
 */
class QrRegExpValidatorPrivate;
class QRWIDGETSSHARED_EXPORT QrRegExpValidator : public QRegExpValidator
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(QrRegExpValidator)

public:
    explicit QrRegExpValidator(QObject *parent = Q_NULLPTR);
    explicit QrRegExpValidator(const QRegExp& rx, QObject *parent = Q_NULLPTR);

    // QValidator interface
public:
    virtual State validate(QString &, int &) const override;

public:
    void setTips(const QString& tips);
};

NS_QRWIDGETS_END

#endif // QRREGEXPVALIDATOR_H
