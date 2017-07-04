#ifndef WIDGETUNREGISTER_H
#define WIDGETUNREGISTER_H

#include <QDialog>

namespace Ui {
class WidgetUnregister;
}

class WidgetUnregister : public QDialog
{
    Q_OBJECT

public:
    explicit WidgetUnregister(QWidget *parent = 0);
    ~WidgetUnregister();

    void setLabelText( QString str );

public slots:
    void unregister();

public:
    Ui::WidgetUnregister *ui;
};

#endif // WIDGETUNREGISTER_H
