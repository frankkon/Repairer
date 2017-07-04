#ifndef WIDGETOPTION_H
#define WIDGETOPTION_H

#include <QWidget>

namespace Ui {
class WidgetOption;
}

class WidgetOption : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetOption(QWidget *parent = 0);
    ~WidgetOption();

public slots:
    void apply();

private:
    Ui::WidgetOption *ui;
};

#endif // WIDGETOPTION_H
