#include "widgetleft.h"

#include <QHBoxLayout>
#include <QSignalMapper>
#include <QPalette>
#include <QBitmap>
#include <QMessageBox>
#include "global.h"
#include "dlgregisterkey.h"
#include "widgetunregister.h"


WidgetLeft::WidgetLeft(QWidget *parent) :
    QWidget(parent)
{
    m_nCurrentButton = 0;
    m_pBtnRegister = new QPushButton();
    QPixmap pixmap(":/main/reg");
    m_pBtnRegister->setIcon(pixmap);
    m_pBtnRegister->setIconSize(pixmap.size());
    m_pBtnRegister->setFlat(true);
    m_pBtnRegister->setCursor(Qt::PointingHandCursor);
    //m_pBtnRegister->setMask(pixmap.createHeuristicMask());
    m_pBtnRegister->setFocusPolicy(Qt::NoFocus); //隐藏虚框
    m_pBtnRegister->setStyleSheet("background-color:transparent;");

    //容器背景
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(Global::s_clrWidgetLeft) );
    setPalette(palette);
    setAutoFillBackground(true);

    //设置左侧容器宽度
    setFixedWidth(240);

    QVBoxLayout* button_layout = new QVBoxLayout();
    button_layout->setSpacing(3);

    //设置按钮
    QStringList string_list, string_list2;
    string_list<<":/main/home"<<":/main/pcScan"<<":/main/fileRepair"<<":/main/diskClean"<<":/main/settings";
    string_list2<<":/main/home2"<<":/main/pcScan2"<<":/main/fileRepair2"<<":/main/diskClean2"<<":/main/settings2";

    QSignalMapper* signal_mapper = new QSignalMapper(this);
    for( int i=0; i<string_list.size(); i++ )
    {
        QPushButton* pButton = new QPushButton();
        m_pBtn[i] = pButton;
//        pButton->setFixedSize( 234, 62 );
//        QPalette pal = pButton->palette();
//        pal.setColor(QPalette::Button,QColor(119,144,174));
//        pButton->setPalette(pal);

        QPixmap pixmap(string_list.at(i));
        QPixmap pixmap2(string_list2.at(i));
        m_pixmap[i] = pixmap;
        m_pixmap2[i] = pixmap2;
        pButton->setIcon(pixmap);
        pButton->setIconSize(pixmap.size());
        pButton->setFixedSize(pixmap.size());
        pButton->setCursor(Qt::PointingHandCursor);
        pButton->setFocusPolicy(Qt::NoFocus); //隐藏虚框

        m_lstBtn.append(pButton);
        connect( pButton, SIGNAL(clicked()), signal_mapper, SLOT(map()) );
        signal_mapper->setMapping( pButton, QString::number(i, 10) );

        button_layout->addWidget( pButton, 0, Qt::AlignLeft );
        //button_layout->setSpacing(5);
    }
    m_pBtn[0]->setIcon(m_pixmap2[0]);
    button_layout->addWidget( m_pBtnRegister, 0, Qt::AlignBottom | Qt::AlignHCenter );
    button_layout->setContentsMargins( 4, 18, 0, 5 );
    setLayout(button_layout);

    connect( signal_mapper, SIGNAL(mapped(QString)), this, SLOT(changePage(QString)) );
    connect( m_pBtnRegister, &QPushButton::clicked, this, &WidgetLeft::registerDlg );
}

void WidgetLeft::changePage( QString strPage )
{
    bool bOk;
    int nIndex = strPage.toInt( &bOk, 10 );

//    for( int i=0; i<m_lstBtn.count(); i++ )
//    {
//        QPushButton* pBtn = m_lstBtn.at(i);
//        if( current_index == i )
//        {
//            tool_button->setMousePress(true);
//        }
//        else
//        {
//            tool_button->setMousePress(false);
//        }
//    }

    setButtonPng(nIndex);
    emit changePage(nIndex);
}

void WidgetLeft::setButtonPng( int nIndex )
{
    if( m_nCurrentButton == nIndex )
    {
        return;
    }

    m_pBtn[nIndex]->setIcon(m_pixmap2[nIndex]);
    m_pBtn[m_nCurrentButton]->setIcon(m_pixmap[m_nCurrentButton]);
    m_nCurrentButton = nIndex;
}

void WidgetLeft::registerDlg()
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
