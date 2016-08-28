#ifndef EDITABLELABELWIDGET_H
#define EDITABLELABELWIDGET_H

#include <QWidget>

namespace Ui {
class EditableLabelWidget;
}

class EditableLabelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditableLabelWidget(QWidget *parent = 0);
    ~EditableLabelWidget();

private:
    Ui::EditableLabelWidget *ui;
};

#endif // EDITABLELABELWIDGET_H
