#include "qrregexpvalidator.h"

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qtooltip.h>

NS_QRWIDGETS_BEGIN
class QrRegExpValidatorPrivate{
public:
    QString tips;
};
NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

QrRegExpValidator::QrRegExpValidator(QObject *parent)
    : QRegExpValidator(parent),
      d_ptr(new QrRegExpValidatorPrivate)
{

}

QrRegExpValidator::QrRegExpValidator(const QRegExp &rx, QObject *parent)
    : QRegExpValidator(rx, parent),
      d_ptr(new QrRegExpValidatorPrivate)
{

}

QValidator::State QrRegExpValidator::validate(QString &string, int &pos) const
{
    Q_D(const QrRegExpValidator);

    auto state = QRegExpValidator::validate(string, pos);
    if (QValidator::State::Invalid == state) {
        QLineEdit* parentWidget = qobject_cast<QLineEdit*>(parent());
        if (nullptr == parentWidget) {
            return state;
        }

        QFontMetrics fontMetrics(parentWidget->font());
        auto pos = parentWidget->mapToGlobal(parentWidget->pos())
                - parentWidget->pos();
        pos.setX(pos.x() + fontMetrics.width(string));
        pos.setY(pos.y() + fontMetrics.height());

        QToolTip::showText(pos, d->tips);
    } else {
        QToolTip::hideText();
    }

    return state;
}

void QrRegExpValidator::setTips(const QString &tips)
{
    Q_D(QrRegExpValidator);
    d->tips = tips;
}
