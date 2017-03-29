#include "titlebar/qrtitlebar.h"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qstyle.h>

#include <QtCore/qsettings.h>

#include "titlebar/qrshaderdelegate.h"

class QrTitleBarPrivate{
public:
    QR_DECLARE_PUBLIC(QrTitleBar)

public:
    QPushButton* minBtn = nullptr;
    QPushButton* normalBtn = nullptr;
    QPushButton* maxBtn = nullptr;
    QPushButton* closeBtn = nullptr;

    QLabel *logo = nullptr;
    QLabel *title = nullptr;

    const QString geometrySettingKey = "qr_titlebar_layout_geometry";

    QSettings setting;

public:
    QrTitleBarPrivate(QrTitleBar* q);

public:
    void init();
    void initForMac(QHBoxLayout *layout);
    void initForWindows(QHBoxLayout *layout);
};

QrTitleBarPrivate::QrTitleBarPrivate(QrTitleBar *q)
    : q_ptr(q)
{
}

void QrTitleBarPrivate::init()
{
    Q_Q(QrTitleBar);

    QHBoxLayout *layout = new QHBoxLayout;

    minBtn = new QPushButton(q);
    minBtn->setIcon(q->style()->standardIcon(QStyle::SP_TitleBarMinButton));
    minBtn->setStyleSheet(minBtn->styleSheet() + ";border:none;");
    q->connect(minBtn, &QPushButton::clicked, [this](){
        Q_Q(QrTitleBar);
        q->parentWidget()->showMinimized();
    });

    maxBtn = new QPushButton(q);
    maxBtn->setIcon(q->style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    maxBtn->setStyleSheet(maxBtn->styleSheet() + ";border:none;");
    q->connect(maxBtn, &QPushButton::clicked, [this](){
        maxBtn->hide();
        normalBtn->show();

        Q_Q(QrTitleBar);
        setting.setValue(geometrySettingKey, q->parentWidget()->geometry());

        QRect maxRect = qApp->desktop()->availableGeometry();
        q->parentWidget()->setGeometry(maxRect);
    });

    normalBtn = new QPushButton(q);
    normalBtn->setIcon(q->style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    normalBtn->setStyleSheet(normalBtn->styleSheet() + ";border:none;");
    q->connect(normalBtn, &QPushButton::clicked, [this](){
        normalBtn->hide();
        maxBtn->show();

        Q_Q(QrTitleBar);
        q->parentWidget()->setGeometry(setting.value(geometrySettingKey).toRect());
    });

    closeBtn = new QPushButton(q);
    closeBtn->setIcon(q->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeBtn->setStyleSheet(closeBtn->styleSheet() + ";border:none;");
    q->connect(closeBtn, &QPushButton::clicked, [this](){
        Q_Q(QrTitleBar);
        q->parentWidget()->close();
    });

    normalBtn->hide();
    q->donotMove(minBtn);
    q->donotMove(maxBtn);
    q->donotMove(normalBtn);
    q->donotMove(closeBtn);

    logo = new QLabel(q);
    title = new QLabel(q);

#ifdef Q_OS_WIN
    initForWindows(layout);
#elif Q_OS_MAC
    initForMac(layout);
#else
    qWarning() << "unknown platform for titlebar."
    initForWindows(layout);
#endif

    q->setLayout(layout);
}

void QrTitleBarPrivate::initForMac(QHBoxLayout *layout)
{
    layout->addWidget(closeBtn);
    layout->addWidget(minBtn);
    layout->addWidget(maxBtn);
    layout->addWidget(normalBtn);

    layout->addStretch(1);
    layout->addWidget(logo);
    layout->addWidget(title);
    layout->addStretch(1);
    layout->addSpacing(closeBtn->rect().width());
}

void QrTitleBarPrivate::initForWindows(QHBoxLayout *layout)
{
    layout->addWidget(logo);
    layout->addWidget(title);
    layout->addStretch(1);
    layout->addWidget(minBtn);
    layout->addWidget(maxBtn);
    layout->addWidget(normalBtn);
    layout->addWidget(closeBtn);
}

///////////////////////////////////////////////

QrTitleBar::QrTitleBar(QWidget *parent)
    : QrMovable(parent),
      d_ptr(new QrTitleBarPrivate(this))
{
    Q_ASSERT(nullptr != parent);

    parent->setWindowFlags(Qt::FramelessWindowHint);

    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::WindowSystemMenuHint |
                   Qt::WindowMinMaxButtonsHint);

    Q_D(QrTitleBar);
    d->init();
}

QrTitleBar::~QrTitleBar()
{

}

void QrTitleBar::setFlags(QrTitleBar::BarFlags flags)
{
    Q_D(QrTitleBar);
    d->logo->setVisible(flags & BarFlags::Logo);
    d->title->setVisible(flags & BarFlags::Title);
    d->minBtn->setVisible(flags & BarFlags::MinButton);
    d->maxBtn->setVisible(flags & BarFlags::MaxButton);
    d->normalBtn->setVisible(flags & BarFlags::MaxButton);
    d->closeBtn->setVisible(flags & BarFlags::CloseButton);
}

void QrTitleBar::setTitle(const QString &title)
{
    Q_D(QrTitleBar);
    d->title->setText(title);
}

void QrTitleBar::setLogo(const QPixmap &logo)
{
    Q_D(QrTitleBar);
    d->logo->setPixmap(logo);
}

void QrTitleBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_D(QrTitleBar);
    if(e->button() == Qt::LeftButton) {
        if(d->maxBtn->isVisible()) {
            d->maxBtn->click();
        } else if(d->normalBtn->isVisible()) {
            d->normalBtn->click();
        }
    }

    QWidget::mouseDoubleClickEvent(e);
}
