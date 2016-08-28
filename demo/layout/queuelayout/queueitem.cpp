#include "queueitem.h"

#include <QtCore/QTime>
#include <QtCore/qdebug.h>
#include <QtWidgets/qstyle.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qapplication.h>

int QueueItem::count = 0;

QueueItem::QueueItem(QWidget* parent )
    : QWidget(parent)
{
    closeButton = new QPushButton(parent);
    closeButton->setIcon(QApplication::style()->standardIcon((QStyle::SP_DockWidgetCloseButton)));
    connect(closeButton, &QPushButton::clicked, this, &QueueItem::close);

    std::vector<int> rgba;
    qsrand(QTime::currentTime().msec());
    for(int idx=0; idx<4; ++idx) {
        rgba.push_back(qrand() % ((255 + 1) - 0) + 0);
    }
    content = new QLabel(QString("%1 label").arg(count++), parent);
    content->setStyleSheet(QString("border: 1px solid gray; background-color: rgba(%1, %2, %3, %4)")
                           .arg(rgba[0]).arg(rgba[1]).arg(rgba[2]).arg(rgba[3]));

    setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addStretch(1);
    titleLayout->addWidget(closeButton);

    layout->addLayout(titleLayout);
    layout->addWidget(content);

    setLayout(layout);

    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(QSize(100, 100));

    setStyleSheet("border: 2px solid black;");
}
