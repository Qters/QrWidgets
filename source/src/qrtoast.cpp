#include "qrtoast.h"

#include <QtWidgets/qlabel.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qdesktopwidget.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>

NS_QRWIDGETS_BEGIN
class QrToastPrivate{
public:
    QList<int> labelIds;
    QMap<int, QLabel*> labels;
};
NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

QrToast *QrToast::instance()
{
    static QrToast _instance;
    return &_instance;
}

QrToast::QrToast()
    : d_ptr(new QrToastPrivate)
{

}

void QrToast::show(const QString &msg, const QPoint &point, int msecs /*= 3000*/)
{
    Q_D(QrToast);

    QLabel *toast = new QLabel(nullptr, Qt::FramelessWindowHint
                               | Qt::NoDropShadowWindowHint
                               | Qt::Tool
                               | Qt::WindowStaysOnTopHint);
    toast->setStyleSheet("color : black; "
                         "background-color : (255,255,255,0.5);"
                         "text-align : center; "
                         "padding : 5px 10px;");
    toast->setText(msg);
    toast->setTextFormat(Qt::PlainText);
    toast->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(toast, SIGNAL(destroyed()), toast, SLOT(close()));

    toast->move(point - toast->rect().center());
    toast->show();

    int timeId = startTimer(msecs);
    d->labels[timeId] = toast;
    d->labelIds.push_back(timeId);
}

void QrToast::timerEvent(QTimerEvent *e)
{
    Q_D(QrToast);

    auto timeId = e->timerId();
    if (d->labelIds.contains(timeId)) {
        killTimer(timeId);

        d->labels[timeId]->close();

        d->labels.remove(timeId);
        d->labelIds.removeAll(timeId);
    }

    QObject::timerEvent(e);
}
