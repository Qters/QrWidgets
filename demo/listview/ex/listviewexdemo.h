#ifndef LISTVIEWEXDEMO_H
#define LISTVIEWEXDEMO_H

#include <QWidget>

#include "qrglobal.h"

namespace Ui {
class ListViewExDemo;
}

class ListViewExDemoPrivate;
class ListViewExDemo : public QWidget
{
    Q_OBJECT

    QR_DECLARE_PRIVATE(ListViewExDemo)

public:
    explicit ListViewExDemo(QWidget *parent = 0);
    ~ListViewExDemo();

private:
    Ui::ListViewExDemo *ui;
};

#endif // LISTVIEWEXDEMO_H
