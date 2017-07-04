#include "widgethome.h"
#include "global.h"

#include <QLabel>
#include <QVBoxLayout>

WidgetHome::WidgetHome(QWidget *parent) :
    QWidget(parent)
{
    m_pWidgetSysInfo = new WidgetHomeSysInfo();

    //容器背景
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(QColor(105,114,124)) );
    setPalette(palette);
    setAutoFillBackground(true);

    m_pBtnSysScan = new QPushButton();
    QPixmap pixmap(":/home/scan");
    m_pBtnSysScan->setIcon(pixmap);
    m_pBtnSysScan->setIconSize(pixmap.size());
    m_pBtnSysScan->setFlat(true);
    m_pBtnSysScan->setCursor(Qt::PointingHandCursor);
    m_pBtnSysScan->setFocusPolicy(Qt::NoFocus); //隐藏虚框
    m_pBtnSysScan->setStyleSheet("background-color:transparent;");



    initLayout();
}

WidgetHome::~WidgetHome()
{

}

void WidgetHome::initLayout()
{
    //顶部色块
    QLabel* pLabTop = new QLabel();
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(Global::s_clrWidgetLeft) );
    pLabTop->setPalette(palette);
    pLabTop->setFixedHeight(12);

    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->addWidget(pLabTop);
    pLayout->addWidget(m_pWidgetSysInfo);
    pLayout->addWidget( m_pBtnSysScan, 0, Qt::AlignRight );

    pLayout->setContentsMargins( 0, 0, 0, 0 );

    setLayout(pLayout);


}
