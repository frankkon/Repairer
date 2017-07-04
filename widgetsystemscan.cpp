#include "widgetsystemscan.h"
#include "ui_widgetsystemscan.h"


#include <QVBoxLayout>
#include <QThread>
#include <QDebug>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>
#include <QThread>
#include <QTreeWidgetItem>
#include "dlgregisterkey.h"
#include "tool.h"
#include <QString>


WidgetSystemScan::WidgetSystemScan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSystemScan)
{
    ui->setupUi(this);

    m_pObjSysScan = new ObjSystemScan();

    m_pThread = new QThread(this);

//    m_pLabIco = new QLabel();
//    m_pLabTitlePro = new QLabel();
//    m_pLabTitleLst = new QLabel();
//    m_pBtnScan = new QPushButton();
//    m_pProBar = new QProgressBar();
//    m_pTableWidget = new QTableWidget();
//    m_pBtnSelAll = new QPushButton();
//    m_pBtnSelNone = new QPushButton();
//    m_pBtnDownloadAll = new QPushButton();
//    m_pLabWatch = new QLabel();


//    m_pBtnSelAll->setText("Select All");
//    m_pBtnSelNone->setText("Select None");
//    m_pBtnDownloadAll->setText("Fix DLL Errors");

    //设置背景
    QPalette palette;
    palette.setBrush( QPalette::Window, QBrush(QColor(255,255,255)) );
    setPalette(palette);
    setAutoFillBackground(true);

    translate();

    InitControl();
    InitLayout();

    //信号槽
    connect( this, &WidgetSystemScan::sigStartCheckDlls, m_pObjSysScan, &ObjSystemScan::startCheckDlls, Qt::BlockingQueuedConnection );
    connect( m_pObjSysScan, &ObjSystemScan::sigCurrentCheckFile, this, &WidgetSystemScan::updateCurrentCheckFile, Qt::AutoConnection );
    connect( m_pObjSysScan, &ObjSystemScan::sigAddFileToTable, this, &WidgetSystemScan::addFileToTable, Qt::QueuedConnection );
    connect( m_pObjSysScan, &ObjSystemScan::sigDownloadFinished, this, &WidgetSystemScan::downloadFinished, Qt::QueuedConnection );
    connect( m_pObjSysScan, &ObjSystemScan::sigCurrentDownloadFile, this, &WidgetSystemScan::updateCurrentDownloadFile, Qt::QueuedConnection );

//    connect( m_pBtnSelAll, &QPushButton::clicked, this, &WidgetSystemScan::selectAll );
//    connect( m_pBtnSelNone, &QPushButton::clicked, this, &WidgetSystemScan::selectNone );
    connect( ui->pushButton_fix, &QPushButton::clicked, this, &WidgetSystemScan::downloadAll );

    connect( this, &WidgetSystemScan::sigDownloadFile, m_pObjSysScan, &ObjSystemScan::downloadFile, Qt::QueuedConnection );

    m_pObjSysScan->moveToThread(m_pThread);
    m_pThread->start();
}

WidgetSystemScan::~WidgetSystemScan()
{
    delete m_pObjSysScan;
    m_pObjSysScan = NULL;
}

void WidgetSystemScan::InitControl()
{
//    {
//        QPixmap pixmap(":/home/pc");
//        m_pLabIco->setPixmap(pixmap);
//        m_pLabIco->setFixedSize(pixmap.size());
//    }

//    {
//        QPixmap pixmap(":/home/watch");
//        m_pLabWatch->setPixmap(pixmap);
//        m_pLabWatch->setFixedSize(pixmap.size());
//    }

//    //进度条
//    m_pProBar->setFixedWidth(450);


//    //列表
//    m_pTableWidget->setRowCount(0);
//    m_pTableWidget->setColumnCount(6);

//    m_pTableWidget->verticalHeader()->setVisible(false);       //隐藏列表头
//    //m_pTableWidget->horizontalHeader()->setVisible(false);   //隐藏行表头
//    m_pTableWidget->setShowGrid(false);                              //隐藏表格线
//    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    //设置表格行标题
//    QStringList lstHeader;
//    lstHeader<<" "<<" "<<"Name"<<"Path"<<" "<<" ";
//    m_pTableWidget->setHorizontalHeaderLabels(lstHeader);


//    m_pTableWidget->setColumnWidth( 0, 22 );
//    m_pTableWidget->setColumnWidth( 1, 20 );
//    m_pTableWidget->setColumnWidth( 2, 180 );
//    m_pTableWidget->setColumnWidth( 3, 360 );
//    m_pTableWidget->setColumnWidth( 4, 70 );
//    m_pTableWidget->setColumnWidth( 5, 50 );

    QStringList headers;
    headers<<"suffix"<<"name"<<"path"<<" ";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setColumnWidth(0, 70);
    ui->treeWidget->setColumnWidth(1, 220);
    ui->treeWidget->setColumnWidth(2, 250);
    ui->treeWidget->setColumnWidth(3, 140);
    //ui->treeWidget->setColumnWidth(4, 60);
}

void WidgetSystemScan::InitLayout()
{
//    //上层布局
//    QGridLayout* pLayoutTop = new QGridLayout();
//    pLayoutTop->setContentsMargins( 20, 10, 10, 10 );
//    pLayoutTop->addWidget( m_pLabIco, 0, 0, 2, 1, Qt::AlignLeft | Qt::AlignVCenter );
//    pLayoutTop->addWidget( m_pLabTitlePro, 0, 1, Qt::AlignLeft | Qt::AlignVCenter );
//    pLayoutTop->addWidget( m_pProBar, 1, 1, Qt::AlignLeft | Qt::AlignTop );
//    //pLayoutTop->addWidget( m_pBtnScan, 0, 2, 2, 2, Qt::AlignCenter );
//    pLayoutTop->addWidget( m_pLabWatch, 0, 2, 2, 2, Qt::AlignRight );
//    pLayoutTop->setColumnStretch( 1, 5 );
//    pLayoutTop->setHorizontalSpacing(20);

//    //中层布局
//    QVBoxLayout* pLayoutCenter = new QVBoxLayout();
//    pLayoutCenter->setContentsMargins( 0, 0, 0, 0 );
//    pLayoutCenter->addWidget(m_pLabTitleLst);
//    pLayoutCenter->addWidget(m_pTableWidget);

//    //底层布局
//    QHBoxLayout* pLayoutBottom = new QHBoxLayout();
//    pLayoutBottom->addWidget(m_pBtnSelAll);
//    pLayoutBottom->addWidget(m_pBtnSelNone);
//    pLayoutBottom->addWidget(m_pBtnDownloadAll);

//    QVBoxLayout* pLayoutMain = new QVBoxLayout();
//    pLayoutMain->setContentsMargins( 0, 0, 0, 5 );
//    pLayoutMain->addLayout(pLayoutTop);
//    pLayoutMain->addLayout(pLayoutCenter);
//    pLayoutMain->addLayout(pLayoutBottom);
//    setLayout(pLayoutMain);

//    QGridLayout* layout = new QGridLayout();
//    //layout->setSpacing(10);
//    layout->addWidget( m_pLabIco, 0, 0, 2, 1, Qt::AlignCenter );
//    layout->addWidget( m_pLabTitlePro, 0, 1, Qt::AlignCenter );
//    layout->addWidget( m_pProBar, 1, 1, Qt::AlignCenter );

//    layout->addWidget( m_pLabTitleLst, 2, 0, 1, 3, Qt::AlignLeft );
//    layout->addWidget( m_pTableWidget, 3, 0, 1, 3, Qt::AlignLeft );

//    layout->addWidget( m_pBtnSelAll, 4, 0, Qt::AlignCenter );
//    layout->addWidget( m_pBtnSelNone, 4, 1, Qt::AlignCenter );
//    layout->addWidget( m_pBtnDownloadAll, 4, 2, Qt::AlignCenter );

//    layout -> setColumnStretch( 0, 2 );
//    layout -> setColumnStretch( 1, 10 );
//    layout -> setColumnStretch( 2, 20 );

//    layout -> setRowStretch( 0, 1 );
//    layout -> setRowStretch( 1, 1 );
//    layout -> setRowStretch( 2, 1 );
//    layout -> setRowStretch( 3, 10 );
//    layout -> setRowStretch( 4, 1 );

//    m_pTableWidget->resize( 350, 700 );

//    setLayout(layout);
}

void WidgetSystemScan::translate()
{
//    m_pLabTitlePro->setText(tr("System Scan in Progress..."));
//    m_pLabTitleLst->setText(tr("  Error found"));
}

void WidgetSystemScan::updateCurrentCheckFile( QString strFile, int nTotal, int nCurrent )
{
    qDebug()<<"WidgetSystemScan::updateCurrentCheckFile thread:"<<QThread::currentThreadId();
    if( 0 == nCurrent )
    {
        ui->progressBar->setRange( 0, nTotal - 1 );
    }

    ui->progressBar->setValue(nCurrent);

     //qDebug()<<strFile<<"  "<<nCurrent<<"  "<<nTotal;

     strFile = "  " + strFile;
     ui->label_file->setText(strFile);
     ui->label_file->repaint();
}

void WidgetSystemScan::updateCurrentDownloadFile( FileListItem* pItem, int nPercentage )
{
    //qDebug()<<"WidgetSystemScan::CurrentDownloadFile:"<<QThread::currentThreadId();
    if( 0 == nPercentage )
    {
        pItem->m_pPbr->setRange( 0, 100 );
    }
    pItem->m_pPbr->setValue(nPercentage);
}

void WidgetSystemScan::addFileToTable( FileListItem* pItem )
{
//    int nRow = m_pTableWidget->rowCount();
//    m_pTableWidget->insertRow(nRow);
//    m_pTableWidget->setItem( nRow, 0, new QTableWidgetItem( QIcon(":/system_scan/file"), "" )  );
//    QCheckBox* pCheckBox = new QCheckBox();
//    m_pTableWidget->setCellWidget( nRow, 1, pCheckBox );
//    m_pTableWidget->setItem( nRow, 2, new QTableWidgetItem( pItem->m_strFileName ) );
//    {
//        int nIndex1 = pItem->m_strFilePath.indexOf('\\');
//        int nIndex2 = pItem->m_strFilePath.lastIndexOf('\\');
//        QString strTmp = pItem->m_strFilePath.mid( nIndex1+1, nIndex2 - nIndex1 );
//        m_pTableWidget->setItem( nRow, 3, new QTableWidgetItem(strTmp));
//    }

//    QPushButton* pBtnDownload = new QPushButton();
//    pBtnDownload->setText(tr("download"));
//    m_pTableWidget->setCellWidget( nRow, 4, pBtnDownload );

//    QPushButton* pBtnDelete = new QPushButton();
//    pBtnDelete->setText(tr("delete"));
//    m_pTableWidget->setCellWidget( nRow, 5, pBtnDelete );

//    pItem->setThread(m_pThread);
//    pItem->setDownloadButton(pBtnDownload);
//    pItem->setDeleteButton(pBtnDelete);

//    //connect( pCheckBox, &QCheckBox::stateChanged, this, WidgetSystemScan::setCheckBox );
//    connect( pBtnDelete, &QPushButton::clicked, this, &WidgetSystemScan::deleteFileFromTable );
//    connect( pBtnDownload, &QPushButton::clicked, this, &WidgetSystemScan::downloadFile );
//    connect( pItem, &FileListItem::sigCurrentDownloadFile, this, &WidgetSystemScan::updateCurrentDownloadFile );
//    connect( pItem, &FileListItem::sigDownloadFinished, this, &WidgetSystemScan::downloadFinished );

//    QTableWidgetItem* pTableItem = new QTableWidgetItem(QTableWidgetItem::UserType);
//    pTableItem->setData( Qt::DisplayRole, pItem );
//    m_pTableWidget->setItem( nRow, 6, pTableItem );

    //m_pTableWidget->repaint();

    QString strSuffix = Tool::getFileSuffix(pItem->m_strFileName);
    QList<QTreeWidgetItem*> lst = ui->treeWidget->findItems( strSuffix, Qt::MatchFixedString );
    QTreeWidgetItem* pTreeItem = NULL;
    if( 0 == lst.size() )
    {
        QStringList title;
        title<<strSuffix;
        pTreeItem = new QTreeWidgetItem( ui->treeWidget, title );
    }
    else if( 1 == lst.size() )
    {
        pTreeItem = lst.at(0);
    }
    else
    {
        QString strTmp = QString("%1 double").arg(strSuffix);
        QMessageBox::about( this, Global::s_strExeName, strTmp );

        return;
    }

    QStringList list1;
    int nIndex1 = pItem->m_strFilePath.indexOf('\\');
    int nIndex2 = pItem->m_strFilePath.lastIndexOf('\\');
    QString strTmp = pItem->m_strFilePath.mid( nIndex1+1, nIndex2 - nIndex1 );

    list1 <<""<<pItem->m_strFileName<<strTmp;
    QTreeWidgetItem *leaf1 = new QTreeWidgetItem(pTreeItem, list1);
    int nValue = (int)pItem;
    leaf1->setData( 0, Qt::UserRole, QVariant(nValue) );
    pTreeItem->addChild(leaf1);
    ui->treeWidget->expandAll();

    QLabel* pLab = new QLabel();
    pLab->setPixmap(QPixmap(":/system_scan/file"));
    ui->treeWidget->setItemWidget( leaf1, 0, pLab );
    pItem->m_pLabel = pLab;

    QProgressBar* pPbr = new QProgressBar;
    ui->treeWidget->setItemWidget( leaf1, 3, pPbr );
    pItem->m_pPbr = pPbr;
}

void WidgetSystemScan::deleteFileFromTable()
{
//    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());

//    if( pBtn == 0 )
//        return;

//    QModelIndex nIndex = m_pTableWidget->indexAt( QPoint(pBtn->frameGeometry().x(), pBtn->frameGeometry().y()) );
//    int nRow = nIndex.row();


//    FileListItem* pItem = m_pObjSysScan->GetLostFileFromList(nRow);
//    if( E_STATUS_DISPLAY == pItem->m_eStatus )
//    {
//        m_pTableWidget->removeRow(nRow);
//        m_pObjSysScan->deleteFromTable(nRow);
//    }
//    else if( E_STATUS_DOWNLOADING == pItem->m_eStatus )
//    {
//        pItem->cancelDownload();
//        pItem->m_pBtnDownload->setText(tr("download"));
//        pItem->m_pBtnDelete->setText(tr("delete"));
//    }
//    else
//    {
//        m_pTableWidget->removeRow(nRow);
//        m_pObjSysScan->deleteFromTable(nRow);
//    }
}

void WidgetSystemScan::downloadFile()
{
    if(!Global::s_bActive)
    {
        DlgRegisterKey dlg;
        dlg.exec();

        if(!Global::s_bActive)
        {
            return;
        }
    }

    qDebug()<<"WidgetSystemScan::downloadFile"<<QThread::currentThreadId();

//    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());

//    if( pBtn == 0 )
//        return;

//    QModelIndex nIndex = m_pTableWidget->indexAt( QPoint(pBtn->frameGeometry().x(), pBtn->frameGeometry().y()) );
//    int nRow = nIndex.row();

//    FileListItem* pItem = m_pObjSysScan->GetLostFileFromList(nRow);

//    if( E_STATUS_DISPLAY == pItem->m_eStatus )
//    {
//        pItem->downloadFile();
//        pItem->m_pBtnDelete->setText(tr("cancel"));
//    }
//    else if( E_STATUS_LOCAL == pItem->m_eStatus )
//    {
//        pItem->openFile();
//    }
//    else
//    {

//    }
}

void WidgetSystemScan::selectAll()
{
//    int nCount = m_pTableWidget->rowCount();
//    for( int i = 0; i < nCount; i++ )
//    {
//        QCheckBox* pCheckBox = (QCheckBox*)m_pTableWidget->cellWidget( i, 1 );
//        bool bChecked = pCheckBox->isChecked();

//        if(!bChecked)
//        {
//            pCheckBox->setChecked(true);
//        }
//    }
}

void WidgetSystemScan::selectNone()
{
//    int nCount = m_pTableWidget->rowCount();
//    for( int i = 0; i < nCount; i++ )
//    {
//        QCheckBox* pCheckBox = (QCheckBox*)m_pTableWidget->cellWidget( i, 1 );
//        bool bChecked = pCheckBox->isChecked();

//        if(bChecked)
//        {
//            pCheckBox->setChecked(false);
//        }
//    }
}

void WidgetSystemScan::downloadAll()
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    m_pObjSysScan->m_lstFileDownloadTmp.clear();
    while (*it)
    {
        QTreeWidgetItem* p = *it;
        int nValue = p->data(0,Qt::UserRole).toInt();
        FileListItem* pItem = (FileListItem*)nValue;
        if( 0 == p->childCount() )
        {
            m_pObjSysScan->m_lstFileDownloadTmp.push_back(pItem);
        }

        it++;
    }

    m_pObjSysScan->m_nCurIndex = 0;

    if(Global::s_bActive)
    {
        emit sigDownloadFile(0);
    }
    else
    {
        QString strFlag = Global::readFreeFixUnRegister();
        if( strFlag == "1" )
        {
            DlgRegisterKey dlg;
            dlg.exec();
        }
        else
        {
            emit sigDownloadFile(0);
        }
    }
}

void WidgetSystemScan::setCheckBox( int nState )
{
    QCheckBox* pChk = qobject_cast<QCheckBox*>(sender());

    pChk->setChecked(nState);

//    if( pChk == 0 )
//        return;

//    QModelIndex nIndex = m_pTableWidget->indexAt( QPoint(pChk->frameGeometry().x(), pBtn->frameGeometry().y()) );
//    int nRow = nIndex.row();

//    FileListItem* pItem = m_pObjSysScan->GetLostFileFromList(nRow);

//    if( 0 == nState )
//    {

//    }
}

void WidgetSystemScan::downloadFinished( FileListItem* pItem, bool bSuccess )
{
    if(!bSuccess)
    {
        if( pItem->m_strDownloadError.lastIndexOf("Not Found") != -1 )
        {
            //将服务器没有的文件写入配置文件
            QString strPath = QString("%1\\dllescort\\file.dat").arg(Global::s_strProgramDataPath);

            if(!Tool::isFileOrDirExist(strPath))
            {
                Tool::createDirectory(strPath,true);
            }

            QFile file(strPath);
            if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
            {
                qDebug()<<"open file "<<strPath<<" failed,error:"<<file.errorString();
            }
            else
            {

            }

            QTextStream txtInput(&file);

            bool bNew = true;
            bool bNoLine = true;
            while(!txtInput.atEnd())
            {
                QString strLine = txtInput.readLine();
                if( strLine == pItem->m_strFileName )
                {
                    bNew = false;
                    break;
                }
                bNoLine = false;
            }

            if(bNew)
            {
                if(!bNoLine)
                {
                    txtInput<<"\r\n";
                }

                txtInput<<pItem->m_strFileName;
            }

            file.close();
            pItem->m_pPbr->setValue(100);
            pItem->m_pLabel->setPixmap(QPixmap(":/system_scan/exist"));
        }
        else if(pItem->m_strDownloadError.lastIndexOf("Connection timed out") != -1)
        {
            pItem->m_pPbr->setValue(0);
            QMessageBox::about( this, Global::s_strExeName, pItem->m_strDownloadError);
            return;
        }
    }
    else
    {
        pItem->m_pLabel->setPixmap(QPixmap(":/system_scan/exist"));
    }


    if(!Global::s_bActive)
    {
        DlgRegisterKey dlg;
        dlg.exec();
    }
}

void WidgetSystemScan::paintEvent(QPaintEvent * event)
{
    //上部分位图
    QRect rctPic;
    {
        QPixmap pixmap = QPixmap(":/system_scan/bk");
        QPainter painter(this);
        rctPic = QRect( 0, -15, pixmap.width(), pixmap.height() );
        painter.drawPixmap( rctPic, pixmap );
    }

//    //下部分列表
//    {
//        QRect rctLst( rctPic.width(), rctPic.height(), rect().width() - rctPic.width(), rect().height()- rctPic.height() );
//        QPainter painter(this);
//        painter.setBrush(Qt::white);
//        painter.drawRect(rctLst);
//    }
}

void WidgetSystemScan::startCheckDlls()
{
    //m_pObjSysScan->startCheckDlls();
    qDebug()<<"WidgetSystemScan::startCheckDlls:"<<QThread::currentThreadId();
    emit sigStartCheckDlls();
}

void WidgetSystemScan::init()
{
    m_pObjSysScan->ReadDatFile();
}

void WidgetSystemScan::clearLostFile()
{
    ui->treeWidget->clear();
    m_pObjSysScan->clearLostFile();
}
