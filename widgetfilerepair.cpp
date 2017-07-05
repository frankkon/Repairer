#include <QVBoxLayout>
#include <QThread>
#include <QDebug>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>
#include "widgetfilerepair.h"
#include "global.h"
#include "ui_widgetfilerepair.h"


WidgetFileRepair::WidgetFileRepair(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WidgetFileRepair)
{
    ui->setupUi(this);

    m_pObjFileRepair = new ObjFileRepair();

    m_pThread = new QThread(this);

    //    m_pLabIco = new QLabel();
    //    m_pLabTitleEdt = new QLabel();
    //    m_pLabTitleLst = new QLabel();
    //    m_pEdtFile = new QLineEdit();
    //    ui->pushButton_download = new QPushButton();
    //    m_pTableWidget = new QTableWidget();


    //设置背景
    //    QPalette palette;
    //    palette.setBrush( QPalette::Window, QBrush(QColor(255,255,255)) );
    //    setPalette(palette);
    //    setAutoFillBackground(true);

    m_bDownload = false;

    translate();

    initControl();
    initLayout();

    ui->pushButton_delect->setVisible(false);

    connect(ui->pushButton_download, &QPushButton::clicked, this, &WidgetFileRepair::downloadFile);
    connect(this, &WidgetFileRepair::sigDownloadFile, m_pObjFileRepair, &ObjFileRepair::downloadFile, Qt::QueuedConnection);
    connect(m_pObjFileRepair, &ObjFileRepair::sigDownloadFinished, this, &WidgetFileRepair::downloadFinished,
            Qt::QueuedConnection);
    connect(m_pObjFileRepair, &ObjFileRepair::sigUpdateDownload, this, &WidgetFileRepair::updateDownload,
            Qt::QueuedConnection);

    m_pObjFileRepair->moveToThread(m_pThread);
    m_pThread->start();


}

WidgetFileRepair::~WidgetFileRepair()
{
    delete m_pObjFileRepair;
    m_pObjFileRepair = NULL;
}

void WidgetFileRepair::updateDownload(int nPercentage)
{
    QString strText = QString("%1%").arg(nPercentage);
    ui->pushButton_download->setText(strText);
}

void WidgetFileRepair::initControl()
{
    //    QPixmap pixmap(":/system_scan/find_ico");
    //    m_pLabIco->setPixmap(pixmap);
    //    m_pLabIco->setFixedSize(pixmap.size());

    //    m_pEdtFile->setFixedWidth(420);


    //    m_pTableWidget->setRowCount(0);
    //    m_pTableWidget->setColumnCount(5);

    //    m_pTableWidget->verticalHeader()->setVisible(false);
    //    m_pTableWidget->horizontalHeader()->setVisible(false);
    //    m_pTableWidget->setShowGrid(false);
    //    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //    //设置表格行标题
    //    QStringList lstHeader;
    //    lstHeader<<" "<<" "<<"Name"<<" ";
    //    m_pTableWidget->setHorizontalHeaderLabels(lstHeader);

    //    m_pTableWidget->setColumnWidth( 0, 22 );
    //    m_pTableWidget->setColumnWidth( 1, 20 );
    //    m_pTableWidget->setColumnWidth( 2, 560 );
    //    m_pTableWidget->setColumnWidth( 3, 50 );
    //    m_pTableWidget->setColumnWidth( 4, 50 );

    //    ui->treeWidget->header()->hide();
    //    ui->treeWidget->setColumnCount(4);

    //    ui->treeWidget->header()->hide();
    //    ui->treeWidget->setColumnCount(4);
    QStringList headers;
    headers << " " << "version" << "size" << "type" << "";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setColumnWidth(0, 290);
    ui->treeWidget->setColumnWidth(1, 180);
    ui->treeWidget->setColumnWidth(2, 65);
    ui->treeWidget->setColumnWidth(3, 45);
    ui->treeWidget->setColumnWidth(4, 30);


    //    QStringList path;
    //    path<<"mfc42.dll";
    //    QTreeWidgetItem* pItem = new QTreeWidgetItem( ui->treeWidget, path );
    //    QStringList list1;
    //    list1 << ""<<QObject::tr("1.0023") << QObject::tr("242")<<"x86";
    //    QTreeWidgetItem *leaf1 = new QTreeWidgetItem(pItem, list1);
    //    pItem->addChild(leaf1);
    //    QStringList zhangsanMath;
    //    zhangsanMath << ""<<QObject::tr("1.0023") << QObject::tr("242")<<"x86";
    //    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(pItem, zhangsanMath);
    //    pItem->addChild(leaf2);

    //    QPushButton* pBtn = new QPushButton;
    //    pBtn->setText("open");
    //    ui->treeWidget->setItemWidget( leaf1, 4, pBtn );

}

void WidgetFileRepair::initLayout()
{
    //上层布局
    //    QGridLayout* pLayoutTop = new QGridLayout();
    //    pLayoutTop->setContentsMargins( 20, 10, 10, 10 );
    //    pLayoutTop->addWidget( m_pLabIco, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter );
    //    pLayoutTop->addWidget( m_pLabTitleEdt, 0, 1, 1, 2, Qt::AlignLeft );
    //    pLayoutTop->addWidget( m_pEdtFile, 1, 1, Qt::AlignLeft );
    //    pLayoutTop->addWidget( ui->pushButton_download, 0, 2, 2, 1, Qt::AlignCenter );
    //    pLayoutTop->setColumnStretch( 1, 5 );
    //    pLayoutTop->setHorizontalSpacing(20);

    //    //中层布局
    //    QVBoxLayout* pLayoutCenter = new QVBoxLayout();
    //    pLayoutCenter->addWidget(m_pLabTitleLst);
    //    pLayoutCenter->addWidget(m_pTableWidget);

    //    //底层布局
    //    QHBoxLayout* pLayoutBottom = new QHBoxLayout();
    ////    pLayoutBottom->addWidget(m_pBtnSelAll);
    ////    pLayoutBottom->addWidget(m_pBtnSelNone);
    ////    pLayoutBottom->addWidget(ui->pushButton_downloadAll);

    //    QVBoxLayout* pLayoutMain = new QVBoxLayout();
    //    pLayoutMain->addLayout(pLayoutTop);
    //    pLayoutMain->addLayout(pLayoutCenter);
    //    pLayoutMain->addLayout(pLayoutBottom);
    //    setLayout(pLayoutMain);
}

void WidgetFileRepair::translate()
{
    ui->pushButton_download->setText(tr("download"));
    ui->label_2->setText(tr("Enter your file name here. E.g. kernel32.dll"));
    ui->label_3->setText(tr("  download file"));
}

void WidgetFileRepair::downloadFinished(FileListDownload* pDownload, bool bSuccess)
{
    qDebug() << "WidgetFileRepair::downloadFinished() begin";
    m_bDownload = false;
    ui->pushButton_download->setText("download");
    if(!bSuccess)
    {
        //QString strMsg = QString("%1 not found").arg(pDownload->m_strFileName);
        QMessageBox::about(this, Global::s_strExeName, pDownload->m_strError);

        delete pDownload;
        pDownload = NULL;

        qDebug() << "WidgetFileRepair::downloadFinished() end";
        return;
    }

    QStringList path;
    path << pDownload->m_strFileName;
    QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->treeWidget, path);

    for(int i = 0; i < pDownload->m_vctVersionX86.size(); i++)
    {
        QString strSize = QString("%1kb").arg(pDownload->m_vctVersionX86[i]->m_fSizeKb);
        QStringList list1;
        list1 << "" << pDownload->m_vctVersionX86[i]->m_strFileVersion << strSize << "X86";
        QTreeWidgetItem* leaf1 = new QTreeWidgetItem(pItem, list1);
        pItem->addChild(leaf1);

        if(!pDownload->m_vctVersionX86[i]->m_bEmpty /*&& !Global::s_bActive*/)
        {
            QLabel* pLab = new QLabel();
            pLab->setPixmap(QPixmap(":/system_scan/exist"));
            ui->treeWidget->setItemWidget(leaf1, 0, pLab);
        }
        else
        {
            QLabel* pLab = new QLabel();
            pLab->setPixmap(QPixmap(":/system_scan/file"));
            ui->treeWidget->setItemWidget(leaf1, 0, pLab);
        }

        QPushButton* pBtn = new QPushButton;
        pBtn->setText("open");
        ui->treeWidget->setItemWidget(leaf1, 4, pBtn);
        connect(pBtn, &QPushButton::clicked, pDownload->m_vctVersionX86[i], &VersionListItem::open, Qt::DirectConnection);
    }

    for(int i = 0; i < pDownload->m_vctVersionX64.size(); i++)
    {
        QString strSize = QString("%1kb").arg(pDownload->m_vctVersionX64[i]->m_fSizeKb);
        QStringList list1;
        list1 << "" << pDownload->m_vctVersionX64[i]->m_strFileVersion << strSize << "X64";
        QTreeWidgetItem* leaf1 = new QTreeWidgetItem(pItem, list1);
        pItem->addChild(leaf1);

        if(!pDownload->m_vctVersionX64[i]->m_bEmpty /*&& !Global::s_bActive*/)
        {
            QLabel* pLab = new QLabel();
            pLab->setPixmap(QPixmap(":/system_scan/exist"));
            ui->treeWidget->setItemWidget(leaf1, 0, pLab);
        }
        else
        {
            QLabel* pLab = new QLabel();
            pLab->setPixmap(QPixmap(":/system_scan/file"));
            ui->treeWidget->setItemWidget(leaf1, 0, pLab);
        }

        QPushButton* pBtn = new QPushButton;
        pBtn->setText("open");
        ui->treeWidget->setItemWidget(leaf1, 4, pBtn);
        connect(pBtn, &QPushButton::clicked, pDownload->m_vctVersionX64[i], &VersionListItem::open, Qt::DirectConnection);
    }

    //ui->treeWidget->expandAll(); //结点全部展开
    ui->treeWidget->collapseAll();
    ui->treeWidget->expandItem(pItem);

    qDebug() << "WidgetFileRepair::downloadFinished() end";

    //    QStringList path;
    //    path<<"mfc42.dll";
    //    QTreeWidgetItem* pItem = new QTreeWidgetItem( ui->treeWidget, path );
    //    QStringList list1;
    //    list1 << QObject::tr("1.0023") << QObject::tr("32");
    //    QTreeWidgetItem *leaf1 = new QTreeWidgetItem(pItem, list1);
    //    pItem->addChild(leaf1);
    //    QStringList zhangsanMath;
    //    zhangsanMath << QObject::tr("数学") << QObject::tr("90");
    //    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(pItem, zhangsanMath);
    //    pItem->addChild(leaf2);

    //    QPushButton* pBtn = new QPushButton;
    //    ui->treeWidget->setItemWidget( leaf1, 3, pBtn );

    //    int nRow = m_pTableWidget->rowCount();
    //    m_pTableWidget->insertRow(nRow);
    //    m_pTableWidget->setItem( nRow, 0, new QTableWidgetItem( QIcon(":/system_scan/file"), "" )  );
    //    QCheckBox* pCheckBox = new QCheckBox();
    //    m_pTableWidget->setCellWidget( nRow, 1, pCheckBox );
    //    m_pTableWidget->setItem( nRow, 2, new QTableWidgetItem( pDownload->m_strFileName ) );

    //    QPushButton* pBtnOpen = new QPushButton();
    //    pBtnOpen->setText(tr("open"));
    //    m_pTableWidget->setCellWidget( nRow, 3, pBtnOpen );

    //    QPushButton* pBtnDelete = new QPushButton();
    //    pBtnDelete->setText(tr("delete"));
    //    m_pTableWidget->setCellWidget( nRow, 4, pBtnDelete );

    //    connect( pBtnDelete, &QPushButton::clicked, this, &WidgetFileRepair::deleteFile );
}

void WidgetFileRepair::downloadFile()
{
    if(m_bDownload)
    {
        return;
    }

    QString strFile = ui->lineEdit->text();
    strFile.remove(' ');

    if(strFile.isEmpty())
    {
        QMessageBox::about(this, Global::s_strExeName, "please input file name");
        return;
    }

    QList<QTreeWidgetItem*> lst = ui->treeWidget->findItems(strFile, Qt::MatchFixedString);
    if(1 == lst.size())
    {
        QString strTmp = QString("%1 already exist, would you want to download?").arg(strFile);
        if(QMessageBox::No == QMessageBox::question(this, Global::s_strExeName, strTmp))
        {
            ui->treeWidget->collapseAll();
            ui->treeWidget->expandItem(lst[0]);
            return;
        }
        else
        {
            int nIndex = ui->treeWidget->indexOfTopLevelItem(lst[0]);
            ui->treeWidget->takeTopLevelItem(nIndex);
        }
    }
    else if(lst.size() > 1)
    {
        QString strTmp = QString("%1 double").arg(strFile);
        QMessageBox::about(this, Global::s_strExeName, strTmp);

        return;
    }
    else
    {
        m_bDownload = true;
    }


    ui->pushButton_download->setText("waiting...");
    emit sigDownloadFile(strFile);
}

void WidgetFileRepair::deleteFile()
{
    //    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());

    //    if( pBtn == 0 )
    //        return;

    //    QModelIndex nIndex = m_pTableWidget->indexAt( QPoint(pBtn->frameGeometry().x(), pBtn->frameGeometry().y()) );
    //    int nRow = nIndex.row();

    //    m_pTableWidget->removeRow(nRow);
}

void WidgetFileRepair::openFile()
{
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());

    if(pBtn == 0)
    { return; }

    QModelIndex nIndex = ui->treeWidget->indexAt(QPoint(pBtn->frameGeometry().x(), pBtn->frameGeometry().y()));
    int nRow = nIndex.row();

    m_pObjFileRepair->openFile(nRow);
}

void WidgetFileRepair::init()
{
    m_pObjFileRepair->init();
}
