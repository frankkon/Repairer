#include "widgethomesysinfo.h"

#include <QGridLayout>
#include "global.h"
#include "ui_WidgetHomeSysInfo.h"

WidgetHomeSysInfo::WidgetHomeSysInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetHomeSysInfo)
{
    ui->setupUi(this);

//    m_pLabPc = new QLabel();
//    m_pLabText1 = new QLabel();
//    m_pLabText2 = new QLabel();
//    m_pLabText3 = new QLabel();
//    m_pLabSysInfo = new QLabel();
//    m_pLabWatch = new QLabel();

//    m_pLabPc->setPixmap(QPixmap(":/home/pc"));
//    m_pLabWatch->setPixmap(QPixmap(":/home/watch"));
//    m_pLabText1->setText(tr("Warning!"));
//    m_pLabText2->setText(tr("You have not yet performed a scan of your system!"));
//    m_pLabText3->setText(tr("Click \"Start Scan Now\" to scan your PC for system issues."));

    QString strText = Global::getSysInfo();
    ui->label->setText(strText);

    //设置上次扫描信息
    QString strTotal = Global::readIniData("LastScanErrorCount");
    QString strFixed = Global::readIniData("LastFixErrorCount");
    QString strLastScanTime = Global::readIniData("LastScanTime");

    QString strLabelTotal        = " LastScanErrorCount : " + strTotal;
    QString strLabelFixed        = "  LastFixErrorCount : " + strFixed;
    QString strLabelLastScanTime = "       LastScanTime : " + strLastScanTime;

    if(!strTotal.isEmpty())
        ui->label_total->setText(strLabelTotal);
    else
        ui->label_total->setText("");

    if(!strFixed.isEmpty())
        ui->label_fixed->setText(strLabelFixed);
    else
        ui->label_fixed->setText("");

    if(!strLastScanTime.isEmpty())
        ui->label_last_scan_time->setText(strLabelLastScanTime);
    else
        ui->label_last_scan_time->setText("");

    //容器背景
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(QColor(255,255,255)) );
    setPalette(palette);
    setAutoFillBackground(true);

    setFixedHeight( 700 - 96 - 18 - 73 );

    initLayout();
}

void WidgetHomeSysInfo::initLayout()
{
//    QVBoxLayout* pLayoutTop1 = new QVBoxLayout();
//    pLayoutTop1->addWidget(m_pLabText1, 0, Qt::AlignHCenter );
//    pLayoutTop1->addWidget(m_pLabText2, 0, Qt::AlignHCenter );


//    //上层布局
//    QHBoxLayout* pLayoutTop = new QHBoxLayout();
//    pLayoutTop->addWidget( m_pLabPc, 0, Qt::AlignLeft );
//    pLayoutTop->addLayout(pLayoutTop1, 2);
//    pLayoutTop->addWidget( m_pLabWatch,0, Qt::AlignRight);

//    //中层布局
//    QVBoxLayout* pLayoutCenter = new QVBoxLayout();   -
//    pLayoutCenter->addWidget(m_pLabTitleLst);
//    pLayoutCenter->addWidget(m_pTableWidget);

//    //底层布局
//    QHBoxLayout* pLayoutBottom = new QHBoxLayout();
//    pLayoutBottom->addWidget(m_pBtnSelAll);
//    pLayoutBottom->addWidget(m_pBtnSelNone);
//    pLayoutBottom->addWidget(m_pBtnDownloadAll);

//    QVBoxLayout* pLayoutMain = new QVBoxLayout();
//    pLayoutMain->addLayout(pLayoutTop);
//    pLayoutMain->addWidget(m_pLabText3, 3, Qt::AlignHCenter);
//    pLayoutMain->addWidget(m_pLabSysInfo, 5,Qt::AlignHCenter);
////    pLayoutMain->addLayout(pLayoutBottom);
//    setLayout(pLayoutMain);

}

void WidgetHomeSysInfo::paintEvent(QPaintEvent * event)
{

}
