#include "widgettop.h"
#include "global.h"
#include "dlgregisterkey.h"
#include <QPalette>
#include <QHBoxLayout>
#include <QBitmap>
#include <QMessageBox>
#include "widgetmain.h"
#include "widgetunregister.h"

WidgetTop::WidgetTop(QWidget *parent) :
    QWidget(parent)
{
    m_pQLabLogo = new QLabel();
    m_pQLabVersion = new QLabel();
    m_pBtnRegister = new QPushButton("Register");
    m_pBtnAbout = new QPushButton("About");
    m_pBtnMinimize = new QPushButton;
    m_pBtnClose = new QPushButton;

    m_pQLabLogo->setPixmap(QPixmap(":/main/logo"));

    QPixmap pixmapMin(":/main/min");
    m_pBtnMinimize->setIcon(QIcon(":/main/min"));
    m_pBtnMinimize->setIconSize(pixmapMin.size());
    m_pBtnMinimize->setFixedSize(pixmapMin.size());
    m_pBtnMinimize->setFlat(true);
    m_pBtnMinimize->setFocusPolicy(Qt::NoFocus); //隐藏虚框
    m_pBtnMinimize->setCursor(Qt::PointingHandCursor);
    //m_pBtnMinimize->setMask(pixmapMin.createHeuristicMask(true));
    m_pBtnMinimize->setStyleSheet("background-color:transparent;");

    QPixmap pixmapClose(":/main/close");
    m_pBtnClose->setIcon(QIcon(":/main/close"));
    m_pBtnClose->setIconSize(pixmapClose.size());
    m_pBtnClose->setFixedSize(pixmapClose.size());
    m_pBtnClose->setFlat(true);
    m_pBtnClose->setFocusPolicy(Qt::NoFocus); //隐藏虚框
    m_pBtnClose->setCursor(Qt::PointingHandCursor);
    //m_pBtnClose->setMask(pixmapClose.createHeuristicMask(true));
    m_pBtnClose->setStyleSheet("background-color:transparent;");


//    QString str = m_pBtnRegister->text();
//    QFont font = m_pBtnRegister->font();
//    QFontMetrics fm(font);
//    int n3 = fm.width(str);
//    int n1 = m_pBtnRegister->fontMetrics().boundingRect(m_pBtnRegister->text()).width();
//    int n2 = m_pBtnRegister->fontMetrics().width(m_pBtnRegister->text());
    m_pBtnRegister->setFixedWidth(m_pBtnRegister->fontMetrics().boundingRect(m_pBtnRegister->text()).width()+10);
    m_pBtnAbout->setFixedWidth(m_pBtnAbout->fontMetrics().boundingRect(m_pBtnAbout->text()).width()+10);
    //m_pBtnRegister->setFixedWidth(n3);

    //m_pBtnMinimize->setFixedSize( 28, 28 );
    //m_pBtnClose->setFixedSize( 28, 28 );

    {
        m_pQLabVersion->setText(Global::s_strVersion);
        QPalette pal = m_pBtnRegister->palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255));
        m_pQLabVersion->setPalette(pal);
    }


    //设置按钮属性
    m_pBtnRegister->setCursor(Qt::PointingHandCursor);
    m_pBtnAbout->setCursor(Qt::PointingHandCursor);
    QPalette pal = m_pBtnRegister->palette();
    pal.setColor(QPalette::ButtonText,QColor(255,255,255));
    pal.setColor(QPalette::Button,Global::s_clrWidgetTop);
    m_pBtnRegister->setPalette(pal);
    m_pBtnAbout->setPalette(pal);
    m_pBtnRegister->setFlat(true);
    m_pBtnAbout->setFlat(true);
    m_pBtnRegister->setFocusPolicy(Qt::NoFocus);
    m_pBtnAbout->setFocusPolicy(Qt::NoFocus);

    //容器背景
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(Global::s_clrWidgetTop) );
    setPalette(palette);
    setAutoFillBackground(true);

    //设置顶层容器高度
    setFixedHeight(96);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget( m_pQLabLogo, 0, Qt::AlignVCenter );
    layout->setSpacing(30);
    //layout->addStretch();
    layout->addWidget( m_pQLabVersion, 0, Qt::AlignVCenter );
    //layout->setSpacing(0);
    layout->addWidget( m_pBtnRegister, 0, Qt::AlignVCenter | Qt::AlignRight  );
    layout->setSpacing(10);
    layout->addWidget( m_pBtnAbout, 0, Qt::AlignVCenter | Qt::AlignLeft  );
    layout->addWidget( m_pBtnMinimize, 0, Qt::AlignTop );
    layout->addWidget( m_pBtnClose, 0, Qt::AlignTop );
    layout->setContentsMargins(15, 0, 5, 0);

    setLayout(layout);

    connect( m_pBtnMinimize, &QPushButton::clicked, this, &WidgetTop::minimizeWidget );
    connect( m_pBtnClose, &QPushButton::clicked, (WidgetMain*)parent, &WidgetMain::hide );
    connect( m_pBtnRegister, &QPushButton::clicked, this, &WidgetTop::registerDlg );
    connect( m_pBtnAbout, &QPushButton::clicked, this, &WidgetTop::about );
}

void WidgetTop::closeWidget()
{
    ((QWidget*)parent())->close();
}

void WidgetTop::minimizeWidget()
{
    ((QWidget*)parent())->showMinimized();
}

void WidgetTop::registerDlg()
{
    if(!Global::s_bActive)
    {
        DlgRegisterKey dlg;
        dlg.exec();
    }
    else
    {
        QString str = QString("Application has been registered, the expiry date is %1-%2-%3").arg(Global::s_seriesNumber.nExpiredYear).arg(Global::s_seriesNumber.nExpiredMonth).arg(Global::s_seriesNumber.nExpiredDay);
        //QMessageBox::about( this, Global::s_strExeName, str );
        WidgetUnregister w;
        w.setLabelText(str);
        w.exec();
    }
}

void WidgetTop::about()
{
    QMessageBox::about( this, Global::s_strExeName, "DLLEscort is DLL Repair Software, fixes .dll missing files, repairs .sys bluescreen problems, resolves .exe high CPU / Memory usage issues and downloads all DLL files. \n\nDLLEscort is the number-one DLL fixer for repairing your Windows PC problems. It fixes DLL errors, cleans and defragments registry, optimizes Windows, restores your system and makes your computer faster and more secure." );
}
