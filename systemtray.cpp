#include "systemtray.h"

SystemTray::SystemTray(QObject* parent) :
    QSystemTrayIcon(parent)
{
    createAction();
    addActions();
    translateLanguage();
}

void SystemTray::createAction()
{
    setIcon(QIcon(":/main/inno"));

    m_pMenu = new QMenu();

    m_pActOpen = new QAction(this);
    m_pActLayout = new QAction(this);

    setContextMenu(m_pMenu);
    m_pMenu->setFixedWidth(100);

    connect(m_pActOpen, SIGNAL(triggered()), this, SIGNAL(showWidget()));
    // BEGIN: Modified by kongjun, 2017/7/8 
    //这里直接使用close退出会出现报错：“thread still running...”
    //connect(m_pActLayout, SIGNAL(triggered()), parent(), SLOT(close()));
    connect(m_pActLayout, SIGNAL(triggered()), parent(), SLOT(onCloseApp()));
    // END:   Modified by kongjun, 2017/7/8 
}

void SystemTray::addActions()
{
    //添加菜单项
    m_pMenu->addAction(m_pActOpen);

    m_pMenu->addSeparator();

    m_pMenu->addAction(m_pActLayout);
}

void SystemTray::translateLanguage()
{
    //放在托盘图标上时候显示
    setToolTip(tr("dllescort"));

    m_pActOpen->setText(tr("Open DLLEscort"));

    m_pActLayout->setText(tr("Exit"));
}
