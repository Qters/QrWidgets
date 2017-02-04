#include "qrfilterlist.h"

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QTimerEvent>

#include "qrchinesetopinyin.h"

USING_NS_QRCOMMON;

NS_QRWIDGETS_BEGIN

class QrFilterListPrivate {
    QR_DECLARE_PUBLIC(QrFilterList)

public:
    int filterTimerId = -1;
    QLineEdit *listEditer = nullptr;
    QListWidget *list = nullptr;

public:
    QrFilterListPrivate(QrFilterList *q)
        : q_ptr(q) {}

public:
    void filterList();
    void initUI();
};

void QrFilterListPrivate::filterList()
{
    Q_Q(QrFilterList);

    emit q->filterStart();

    QRegExp filterReg("*" + listEditer->text() + "*");
    filterReg.setCaseSensitivity(Qt::CaseInsensitive);
    filterReg.setPatternSyntax(QRegExp::Wildcard);

    bool isShow = false;
    for(int idx=0; idx<list->count(); ++idx) {
        QString pinyin = QrChineseToPinYin::convert(
                    list->item(idx)->text().toStdWString(), true).c_str();
        isShow = filterReg.exactMatch(pinyin);
        list->setItemHidden(list->item(idx), !isShow);
    }

    emit q->filterFinish();
}

void QrFilterListPrivate::initUI()
{
    Q_Q(QrFilterList);

    listEditer = new QLineEdit(q);
    list = new QListWidget(q);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(listEditer);
    layout->addWidget(list);

    q->connect(listEditer, &QLineEdit::textChanged, [this](const QString &text){
        Q_UNUSED(text);

        Q_Q(QrFilterList);
        if(-1 != filterTimerId) {
            q->killTimer(filterTimerId);
        }
        filterTimerId = q->startTimer(500);
    });

    q->setLayout(layout);
}

NS_QRWIDGETS_END

/////////////////////////////////////////////////////////////////

USING_NS_QRWIDGETS;

QrFilterList::QrFilterList(QWidget* parent)
    : QWidget(parent),
      d_ptr(new QrFilterListPrivate(this))
{
    Q_D(QrFilterList);
    d->initUI();
}

QListWidget *QrFilterList::listWidget()
{
    Q_D(QrFilterList);
    return d->list;
}

void QrFilterList::timerEvent(QTimerEvent *event)
{
    Q_D(QrFilterList);
    auto timeId = event->timerId();
    killTimer(timeId);

    QObject::timerEvent(event);

    d->filterList();
    d->filterTimerId = -1;
}
