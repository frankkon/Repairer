#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include <QStackedWidget>
#include "dialogdrop.h"
#include "widgettop.h"
#include "widgetleft.h"
#include "widgethome.h"
#include "systemtray.h"
#include "widgetsystemscan.h"
#include "widgetfilerepair.h"
#include "widgetoption.h"
#include "widgetdiskclean.h"

class WidgetMain : public DialogDrop
{
    Q_OBJECT
public:
    explicit WidgetMain(QWidget *parent = 0);

    void init();

signals:

public slots:
    void changePage( int nCurPage );
    void systenScan();

    void showWidget();
    void showMainMenu();
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);

private:
    WidgetTop*        m_pWidgetTop;
    WidgetLeft*       m_pWidgetLeft;
    WidgetHome*       m_pWidgetHome;
    WidgetSystemScan* m_pWidgetSystemScan;
    WidgetFileRepair* m_pWidgetFileRepair;
    WidgetDiskClean*  m_pWidgetDiskClean;
    WidgetOption*     m_pWidgetOption;
    QStackedWidget*   m_pStackWidget;

    SystemTray*       m_pTray;


};

#endif // WIDGETMAIN_H
