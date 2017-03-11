#ifndef QRTITLEBARDEMO_H
#define QRTITLEBARDEMO_H

#include <QWidget>

namespace Ui {
class QrTitlebarDemo;
}

class QrTitlebarDemo : public QWidget
{
    Q_OBJECT

public:
    explicit QrTitlebarDemo(QWidget *parent = 0);
    ~QrTitlebarDemo();

private:
    Ui::QrTitlebarDemo *ui;
};

#endif // QRTITLEBARDEMO_H
