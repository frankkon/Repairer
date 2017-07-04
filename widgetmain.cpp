#include "widgetmain.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>

#define SHADOW_WIDTH       5

WidgetMain::WidgetMain(QWidget *parent) :
    DialogDrop(parent)
{
    m_pWidgetTop = new WidgetTop(this);
    m_pWidgetLeft = new WidgetLeft(this);

    m_pWidgetHome = new WidgetHome();
    m_pWidgetSystemScan = new WidgetSystemScan();
    m_pWidgetFileRepair = new WidgetFileRepair();
    m_pWidgetDiskClean = new WidgetDiskClean();
    m_pWidgetOption = new WidgetOption();
    m_pStackWidget = new QStackedWidget(this);

    m_pTray = new SystemTray(this);

    m_pStackWidget->addWidget(m_pWidgetHome);
    m_pStackWidget->addWidget(m_pWidgetSystemScan);
    m_pStackWidget->addWidget(m_pWidgetFileRepair);
    m_pStackWidget->addWidget(m_pWidgetDiskClean);
    m_pStackWidget->addWidget(m_pWidgetOption);

    //容器背景
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(QColor(0,0,0)) );
    setPalette(palette);
    setAutoFillBackground(true);


    //布局窗口
    QHBoxLayout* pLayoutCenter = new QHBoxLayout();
    pLayoutCenter->addWidget(m_pWidgetLeft);
    pLayoutCenter->addWidget(m_pStackWidget);
    pLayoutCenter->setSpacing(0);
    pLayoutCenter->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* pLayoutMain = new QVBoxLayout();
    pLayoutMain->addWidget(m_pWidgetTop);
    pLayoutMain->addLayout(pLayoutCenter);
    pLayoutMain->setSpacing(0);
    pLayoutMain->setContentsMargins(SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH, SHADOW_WIDTH);

    setLayout(pLayoutMain);

    setMinimumSize(980, 700);

    QDesktopWidget *deskdop = QApplication::desktop();
    move((deskdop->width() - this->width())/2, (deskdop->height() - this->height())/2);

    //设置信号槽
    //connect( m_pWidgetLeft, WidgetLeft::changePage(int), this, WidgetMain::changePage(int) );
    connect( m_pWidgetLeft, SIGNAL(changePage(int)), this, SLOT(changePage(int)) );
    connect( m_pWidgetHome->m_pBtnSysScan, &QPushButton::clicked, this, &WidgetMain::systenScan );

    connect(m_pTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    connect(m_pTray, SIGNAL(showWidget()), this, SLOT(showWidget()));

    m_pTray->show();

    changePage(0);
}

void WidgetMain::changePage( int nCurPage )
{
    switch(nCurPage)
    {
    case 0:
    {
        m_pStackWidget->setCurrentWidget(m_pWidgetHome);
    }
        break;
    case 1:
    {
        m_pStackWidget->setCurrentWidget(m_pWidgetSystemScan);
        m_pWidgetSystemScan->startCheckDlls();
    }
        break;
    case 2:
    {
        m_pStackWidget->setCurrentWidget(m_pWidgetFileRepair);
    }
        break;
    case 3:
    {
        m_pStackWidget->setCurrentWidget(m_pWidgetDiskClean);
    }
        break;
    case 4:
    {
        m_pStackWidget->setCurrentWidget(m_pWidgetOption);
    }
        break;
    default:
    {

    }
        break;
    }
}

void WidgetMain::systenScan()
{
    changePage(1);
}

void WidgetMain::showWidget()
{
    showNormal();
    raise();
    activateWindow();
}

void WidgetMain::showMainMenu()
{
    //设置主菜单出现的位置
//    QPoint p = rect().topRight();
//    p.setX(p.x() - 90);
//    p.setY(p.y() + 25);
//    main_menu->exec(this->mapToGlobal(p));
}

void WidgetMain::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        //点击托盘图标之后松开
    case QSystemTrayIcon::Trigger:
        {
            showWidget();
            break;
        }
        //双击托盘图标
    case QSystemTrayIcon::DoubleClick:
        {
            showWidget();
            break;
        }
    default:
        break;
    }
}

void WidgetMain::init()
{
    m_pWidgetSystemScan->init();
    m_pWidgetFileRepair->init();
}
