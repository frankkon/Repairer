#ifndef WIDGETHOMESYSINFO_H
#define WIDGETHOMESYSINFO_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class WidgetHomeSysInfo;
}

class WidgetHomeSysInfo : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetHomeSysInfo(QWidget *parent = 0);

private:
    void initLayout();

protected:
    void paintEvent(QPaintEvent * event);

signals:

public slots:

private:
    Ui::WidgetHomeSysInfo *ui;

    QLabel* m_pLabPc;
    QLabel* m_pLabText1;
    QLabel* m_pLabText2;
    QLabel* m_pLabText3;
    QLabel* m_pLabSysInfo;
    QLabel* m_pLabWatch;

};

#endif // WIDGETHOMESYSINFO_H
