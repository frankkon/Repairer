#ifndef WIDGETHOME_H
#define WIDGETHOME_H

#include <QWidget>
#include <QPushButton>

#include "widgethomesysinfo.h"

class WidgetHome : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetHome(QWidget *parent = 0);
    ~WidgetHome();

private:
    void initLayout();

signals:

public slots:

public:
    WidgetHomeSysInfo* m_pWidgetSysInfo;
    QPushButton* m_pBtnSysScan;

};

#endif // WIDGETHOME_H
