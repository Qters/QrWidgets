#include "qreditablelabel.h"

#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qlayout.h>
#include <QtCore/qcoreevent.h>
#include <QtGui/qevent.h>
#include <QtCore/qdebug.h>

#include "qrlabel.h"

NS_QRWIDGETS_BEGIN

class QrEditableLabelPrivate{
    QR_DECLARE_PUBLIC(QrEditableLabel)

public:
    QrEditableLabelPrivate(bool editable, QrEditableLabel *q);
    void initLayout();
    void switchState(bool toLabel);

public:
    bool enterToFinish = true;
    bool created = false;
    bool editable = true;

    QRegExp textRegexp;
    QTextEdit *textedit = nullptr;
    QrLabel *label = nullptr;
};

QrEditableLabelPrivate::QrEditableLabelPrivate(bool editable,
                                                   QrEditableLabel *q)
    : editable(editable), q_ptr(q) {}

void QrEditableLabelPrivate::initLayout() {
    Q_Q(QrEditableLabel);

    label = new QrLabel(q);
    label->setCursor(editable ? Qt::PointingHandCursor : Qt::ArrowCursor);
    q->connect(label, &QrLabel::clicked, [this](){
        switchState(false);
    });

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);

    if (editable) {
        textedit = new QTextEdit(q);
        textedit->installEventFilter(q);
        layout->addWidget(textedit);
    }

    q->setLayout(layout);

    switchState(true);
}

void QrEditableLabelPrivate::switchState(bool toLabel) {
    if (! editable) {
        return;
    }

    label->setEnabled(toLabel);
    label->setVisible(toLabel);

    textedit->setEnabled(! toLabel);
    textedit->setVisible(! toLabel);

    if (toLabel) {
        label->setText(textedit->toPlainText());
    } else {
        textedit->setText(label->text());

        QTextCursor lastCursor(textedit->document());
        lastCursor.movePosition(QTextCursor::End);
        textedit->setTextCursor(lastCursor);
        textedit->setFocus();
    }
}

NS_QRWIDGETS_END

/////////////////////////////////////////////////////////////////

USING_NS_QRWIDGETS;

QrEditableLabel::QrEditableLabel(QWidget* parent /*= Q_NULLPTR*/,
                                 bool editable /*= true*/)
    : QWidget(parent),
      d_ptr(new QrEditableLabelPrivate(editable, this))
{
}

void QrEditableLabel::createLayout()
{
    Q_D(QrEditableLabel);
    if (d->created) {
        return;
    }
    d->initLayout();
    d->created = true;
}

void QrEditableLabel::setTextRegex(const QRegExp &regexp)
{
    Q_D(QrEditableLabel);
    d->textRegexp = regexp;
}

void QrEditableLabel::setText(const QString &text)
{
    Q_D(QrEditableLabel);
    if (! d->created) {
        return;
    }
    d->label->setText(text);
}

QString QrEditableLabel::text() const
{
    Q_D(const QrEditableLabel);
    if (! d->created) {
        return "";
    }
    return d->label->text();
}

void QrEditableLabel::handSwitch(bool toLabel)
{
    Q_D(QrEditableLabel);
    d->switchState(toLabel);
}

void QrEditableLabel::setEnterToFinish(bool enterToFinish)
{
    Q_D(QrEditableLabel);
    d->enterToFinish = enterToFinish;
}

void QrEditableLabel::setEditable(bool editable)
{
    Q_D(QrEditableLabel);
    d->editable = editable;
}

bool QrEditableLabel::eventFilter(QObject *target, QEvent *event)
{
    Q_D(QrEditableLabel);
    if(d->editable
            && target == d->textedit ) {
        switch(event->type()) {
        case QEvent::FocusOut:
            if (! d->textRegexp.isEmpty()
                    && ! d->textRegexp.exactMatch(d->textedit->toPlainText())) {
                return QWidget::eventFilter(target, event);
            }

            if (d->label->text() != d->textedit->toPlainText()) {
                emit textChanged(d->textedit->toPlainText());
            }
            d->switchState(true);
            event->accept();
            return true;
            break;
        case QEvent::KeyPress:
            if (d->enterToFinish &&
                    (Qt::Key_Return == static_cast<QKeyEvent*>(event)->key()
                    ||	Qt::Key_Enter == static_cast<QKeyEvent*>(event)->key())) {
                d->switchState(true);
                event->accept();
                return true;
                break;
            }
        }
    }

    return QWidget::eventFilter(target, event);
}
