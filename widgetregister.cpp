#include "widgetregister.h"
#include "global.h"
#include "tool.h"
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

WidgetRegister::WidgetRegister(QWidget *parent) :
    QWidget(parent)
{
    m_pBtnRegister = new QPushButton(this);
    m_pBtnBuy = new QPushButton(this);
    m_pBtnClose = new QPushButton(this);
    m_pEdtKey = new QLineEdit(this);

    {
        QPixmap pixmap(":/register/close");
        m_pBtnClose->setIcon(QIcon(pixmap));
        m_pBtnClose->setIconSize(pixmap.size());
        m_pBtnClose->setFlat(true);
        m_pBtnClose->setFocusPolicy(Qt::NoFocus); //隐藏虚框
        m_pBtnClose->setCursor(Qt::PointingHandCursor);
        //m_pBtnClose->setMask(pixmapMin.createHeuristicMask(true));
        m_pBtnClose->setStyleSheet("background-color:transparent;");
    }

    {
        QPixmap pixmap(":/register/reg");
        m_pBtnRegister->setIcon(QIcon(pixmap));
        m_pBtnRegister->setIconSize(pixmap.size());
        m_pBtnRegister->setFlat(true);
        m_pBtnRegister->setFocusPolicy(Qt::NoFocus); //隐藏虚框
        m_pBtnRegister->setCursor(Qt::PointingHandCursor);
        //m_pBtnRegister->setMask(pixmapMin.createHeuristicMask(true));
        m_pBtnRegister->setStyleSheet("background-color:transparent;");
    }

    {
        QPixmap pixmap(":/register/buy");
        m_pBtnBuy->setIcon(QIcon(pixmap));
        m_pBtnBuy->setIconSize(pixmap.size());
        m_pBtnBuy->setFlat(true);
        m_pBtnBuy->setFocusPolicy(Qt::NoFocus); //隐藏虚框
        m_pBtnBuy->setCursor(Qt::PointingHandCursor);
        //m_pBtnBuy->setMask(pixmapMin.createHeuristicMask(true));
        m_pBtnBuy->setStyleSheet("background-color:transparent;");
    }

    QPixmap pixmap(":/register/bk");
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(pixmap) );
    setPalette(palette);
    setAutoFillBackground(true);

    m_pEdtKey->setGeometry( 111, 216, 563, 33 );
    m_pBtnRegister->setGeometry( 253, 266, 218, 45 );
    m_pBtnBuy->setGeometry( 253, 442, 218, 45 );
    m_pBtnClose->setGeometry( 741, 8, 23, 22 );

    connect( m_pBtnBuy, &QPushButton::clicked, this, &WidgetRegister::buy );
    connect( m_pBtnRegister, &QPushButton::clicked, this, &WidgetRegister::registerKey );
    connect( m_pBtnClose, &QPushButton::clicked, this, &WidgetRegister::closeDialog );

}

void WidgetRegister::buy()
{
    QDesktopServices::openUrl(QUrl("http://www.dllescort.com/order/"));
}

void WidgetRegister::closeDialog()
{
    ((QWidget*)parent())->close();
}

void WidgetRegister::registerKey()
{
    QString strSeriesNumber = m_pEdtKey->text();
    if(!Global::decodeSeriesNumber(strSeriesNumber))
    {
        QMessageBox::about( this, Global::s_strExeName,"invalid key!");
        return;
    }

    QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("Option");
    settings.setValue("SeriesNumber", strSeriesNumber );
    settings.endGroup();

    //备份的路径
    {
        QString strPath = QString("%1\\dllescort\\option.ini").arg(Global::s_strProgramDataPath);
        if(!Tool::isFileOrDirExist(strPath))
        {
            Tool::createDirectory(strPath,true);
        }

        QSettings settings(strPath, QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup("Option");
        settings.setValue("SeriesNumber", strSeriesNumber );
        settings.endGroup();
    }

    QMessageBox::about( this, Global::s_strExeName,"Thank you! Your product registration was successful! Click OK to continue.");
    closeDialog();
}
