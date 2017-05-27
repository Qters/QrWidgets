#ifndef LISTCELLINFO_H
#define LISTCELLINFO_H

#include <QWidget>

namespace Ui {
class ListCellInfo;
}

class ListCellInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ListCellInfo(QWidget *parent = 0);
    ~ListCellInfo();

public:
    void init(const QString& username, const QString& selfsign);

private:
    Ui::ListCellInfo *ui;
};

#endif // LISTCELLINFO_H
