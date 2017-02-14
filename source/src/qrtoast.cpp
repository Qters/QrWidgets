#include "qrtoast.h"

#include <QtWidgets/qlabel.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qdesktopwidget.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>

#include <QtCore/qdebug.h>

NS_QRWIDGETS_BEGIN

class QrToastPrivate{
    QR_DECLARE_PUBLIC(QrToast)

public:
    QrToastPrivate(QrToast* q);

public:

    int maxShowCount = 3;
    int verSpaceOfToast = 5;

    QMap<QWidget*, QVector<QrToastInfo> > waitingToasts;
    QMap<QWidget*, QVector<QrToastInfo> > showingToasts;

    QMap<int, QrToastInfo> timeIdMapToast;

    void showToast(const QrToastInfo& toastInfo);
    void showNextToast(QWidget* widget);
    void pushToast(const QrToastInfo &toastInfo);

    void removeToastBy(int timeId);

    void updateToastPoses(QLabel *curToast, const QrToastInfo &curToastInfo);
    void updateExistToastPoses(QWidget *widget);
};

QrToastPrivate::QrToastPrivate(QrToast *q)
    : q_ptr(q)
{

}

void QrToastPrivate::removeToastBy(int timeId)
{
    if (! timeIdMapToast.contains(timeId) ) {
        return;
    }

    QrToastInfo toastInfo = timeIdMapToast[timeId];
    timeIdMapToast.remove(timeId);

    if (nullptr != toastInfo.toast) {
        if(showingToasts.contains(toastInfo.widget)) {
            QVector<QrToastInfo> existToasts = showingToasts[toastInfo.widget];
            for(int idx=existToasts.size()-1; idx>=0; --idx) {
                auto tempToastInfo = existToasts.at(idx);
                if(toastInfo.toast == tempToastInfo.toast){
                    existToasts.remove(idx);
                }
            }
            if(existToasts.isEmpty()) {
                showingToasts.remove(toastInfo.widget);
            } else {
                showingToasts[toastInfo.widget] = existToasts;
            }
        }

        toastInfo.toast->close();
        delete toastInfo.toast;
        toastInfo.toast = nullptr;
    }

    showNextToast(toastInfo.widget);
}

void QrToastPrivate::updateExistToastPoses(QWidget* widget)
{
    if(nullptr == widget) {
        return;
    }

    int totalHeight = 0;
    QVector<int> existHeights;
    QVector<QrToastInfo> existToasts = showingToasts[widget];
    if(existToasts.isEmpty()) {
        return;
    }
    Q_FOREACH(QrToastInfo toastInfo, existToasts) {
        QLabel* existToast = toastInfo.toast;
        if(nullptr == existToast) {
            continue;
        }
        existHeights.push_back(existToast->height());
        totalHeight += existToast->height();
    }
    totalHeight += existHeights.size() * verSpaceOfToast;

    int toastYPos = widget->mapToGlobal(widget->rect().center()).y() - totalHeight/2;    //  the y pos of the first toast
    Q_FOREACH(QrToastInfo toastInfo, existToasts) {   //  update showing toasts's poses
        QLabel* existToast = toastInfo.toast;
        if(nullptr == existToast) {
            continue;
        }

        QPoint rawPos = existToast->pos();
        rawPos.setY(toastYPos);
        existToast->move(rawPos);

        toastYPos += verSpaceOfToast + existToast->height();
    }
}

void QrToastPrivate::updateToastPoses(QLabel *curToast, const QrToastInfo& curToastInfo)
{
    if(nullptr == curToast) {
        return;
    }

    QFontMetrics fontMetrics(curToast->font());
    QSize curToastSize = fontMetrics.size(Qt::TextSingleLine, curToastInfo.message);

    QPoint curToastPos;
    QRect curToastWidgetRect;
    curToastInfo.calcPos(curToastSize, &curToastPos, &curToastWidgetRect);

    int totalHeight = 0;
    QVector<int> existHeights;
    QVector<QrToastInfo> existToasts = showingToasts[curToastInfo.widget];
    Q_FOREACH(QrToastInfo toastInfo, existToasts) {
        QLabel* existToast = toastInfo.toast;
        if(nullptr == existToast) {
            continue;
        }
        existHeights.push_back(existToast->height());
        totalHeight += existToast->height();
    }
    totalHeight += curToastSize.height() + existHeights.size() * verSpaceOfToast;

    int toastYPos = curToastPos.y() - totalHeight/2;    //  the y pos of the first toast
    Q_FOREACH(QrToastInfo toastInfo, existToasts) {   //  update showing toasts's poses
        QLabel* existToast = toastInfo.toast;
        if(nullptr == existToast) {
            continue;
        }

        QPoint rawPos = existToast->pos();
        rawPos.setY(toastYPos);
        existToast->move(rawPos);

        toastYPos += verSpaceOfToast + existToast->height();
    }
    curToastPos.setY(toastYPos);
    curToast->move(curToastPos);
}

void QrToastPrivate::showToast(const QrToastInfo& toastInfo)
{
    QLabel *curToast = new QLabel(nullptr, Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint
                               | Qt::Tool | Qt::WindowStaysOnTopHint);
    curToast->setObjectName("toast");
    curToast->setText(toastInfo.message);
    curToast->setTextFormat(Qt::PlainText);
    curToast->setStyleSheet("color : white; "
                            "background: rgba(0, 0, 0, 0.5);"
                            "text-align : center; "
                            "padding : 5px 10px;");

    updateToastPoses(curToast, toastInfo);

    curToast->show();

    QrToastInfo newToastInfo = toastInfo;
    newToastInfo.toast = curToast;
    showingToasts[toastInfo.widget].push_back(newToastInfo);

    Q_Q(QrToast);
    int timeId = q->startTimer(toastInfo.timeout);
    timeIdMapToast[timeId] = newToastInfo;
}

void QrToastPrivate::showNextToast(QWidget* widget)
{
    if(! waitingToasts.contains(widget)) {
        updateExistToastPoses(widget);
        return;
    }

    auto toastInfos = waitingToasts[widget];
    if(toastInfos.isEmpty()) {
        return;
    }

    showToast(toastInfos.first());
    toastInfos.removeFirst();

    if(! toastInfos.isEmpty()) {
        waitingToasts[widget] = toastInfos;
    } else {
        waitingToasts.remove(widget);
    }
}

void QrToastPrivate::pushToast(const QrToastInfo& toastInfo)
{
    if(! showingToasts.contains(toastInfo.widget)) {
        showToast(toastInfo);
    } else {
        auto toastInfos = showingToasts[toastInfo.widget];
        if(toastInfos.size() >= maxShowCount) {
            waitingToasts[toastInfo.widget].push_back(toastInfo);
        } else {
            showToast(toastInfo);
        }
    }
}

NS_QRWIDGETS_END

USING_NS_QRWIDGETS;

///////////////////////////////////////////////////////////

void QrToastInfo::calcPos(QSize toastSize, QPoint *toastPos, QRect *widgetRect) const
{
    Q_ASSERT(nullptr != toastPos);
    Q_ASSERT(nullptr != widgetRect);

    *widgetRect = qApp->desktop()->screen(qApp->desktop()->primaryScreen())->rect();
    *toastPos = widgetRect->center();

    if(nullptr != widget) {
        *widgetRect = widget->rect();
        *toastPos = widget->mapToGlobal(widgetRect->center());
    }

    toastPos->setX(toastPos->x() - toastSize.width()/2);
    toastPos->setY(toastPos->y() - toastSize.height()/2);
}

///////////////////////////////////////////////////////////

QrToast *QrToast::instance()
{
    static QrToast _instance;
    return &_instance;
}

QrToast::QrToast()
    : d_ptr(new QrToastPrivate(this))
{

}

QrToast::~QrToast()
{
    Q_D(QrToast);
    for(QMap<int, QrToastInfo>::Iterator iter = d->timeIdMapToast.begin();
        iter != d->timeIdMapToast.end();
        ++iter) {
        d->removeToastBy(iter.key());
    }
}

void QrToast::showInScreen(const QString &msg, int msecs /*= 1500*/)
{
    QrToastInfo toastInfo;
    toastInfo.message = msg;
    toastInfo.timeout = msecs;

    Q_D(QrToast);
    d->pushToast(toastInfo);
}

void QrToast::showInWidget(QWidget *widget, const QString &msg, int msecs)
{
    QrToastInfo toastInfo;
    toastInfo.message = msg;
    toastInfo.timeout = msecs;
    toastInfo.widget = widget;

    Q_D(QrToast);
    d->pushToast(toastInfo);
}

bool QrToast::eventFilter(QObject *watched, QEvent *event)
{
    if(QEvent::Destroy == event->type()) {
        Q_D(QrToast);
        QWidget* widget = qobject_cast<QWidget*>(watched);
        if(nullptr != widget && d->waitingToasts.contains(widget)) {
            d->waitingToasts.remove(widget);
        }
    }
    return QObject::eventFilter(watched, event);
}

void QrToast::timerEvent(QTimerEvent *e)
{
    Q_D(QrToast);

    auto timeId = e->timerId();
    killTimer(timeId);
    d->removeToastBy(timeId);

    QObject::timerEvent(e);
}
