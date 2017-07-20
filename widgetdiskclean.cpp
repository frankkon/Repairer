#include "widgetdiskclean.h"

#include "ui_widgetdiskclean.h"
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QMovie>
#include "global.h"

#define WINDOW_WIDTH 740
#define WINDOW_HEIGHT 592
#define TABLE_WIDTH (WINDOW_WIDTH-10-10)
#define TABLE_HEIGHT (WINDOW_HEIGHT-300-100)

WidgetDiskClean::WidgetDiskClean(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WidgetDiskClean)
{
    ui->setupUi(this);

    initUI();

    //初始化后台扫描线程
    m_pObjDiskClean = new ObjDiskClean();
    m_pThread = new QThread();
    m_pObjDiskClean->moveToThread(m_pThread);
    m_pThread->start();

    //设置信号槽
    //触发一键扫描信号处理
    connect(ui->pushButtonStartScan, &QPushButton::clicked,
            m_pObjDiskClean, &ObjDiskClean::scanAllErrors);

    //触发一键清理信号处理
    connect(ui->pushButtonCleanAll, &QPushButton::clicked,
            m_pObjDiskClean, &ObjDiskClean::cleanAllErrors);

    //触发通知界面更新当前扫描进展
    connect(m_pObjDiskClean, &ObjDiskClean::sigNotifyUIUpdateScanProgress,
            this, &WidgetDiskClean::as_objDiskClean_NotifyUIUpdateScanProgress);

    //触发通知界面更新当前清理进展
    connect(m_pObjDiskClean, &ObjDiskClean::sigNotifyUIUpdateCleanProgress,
            this, &WidgetDiskClean::as_objDiskClean_NotifyUIUpdateCleanProgress);

    //触发通知界面完成所有错误扫描
    connect(m_pObjDiskClean, &ObjDiskClean::sigNotifyUIScanFinished,
            this, &WidgetDiskClean::as_objDiskClean_NotifyUIScanFinished);

    //触发通知界面完成所有错误清理
    connect(m_pObjDiskClean, &ObjDiskClean::sigNotifyUICleanFinished,
            this, &WidgetDiskClean::as_objDiskClean_NotifyUICleanFinished);

}

WidgetDiskClean::~WidgetDiskClean()
{
    if(NULL != m_pObjDiskClean)
    {
        delete m_pObjDiskClean;
        m_pObjDiskClean = NULL;
    }
}

//处理扫描过程中更新UI扫描进展信号
void WidgetDiskClean::as_objDiskClean_NotifyUIUpdateScanProgress(qint64 fileCount,
                                                                 QString fileDesc,
                                                                 QString currentFile)
{
    qDebug() << "处理扫描过程中更新UI扫描进展信号:on_objDiskClean_NotifyUIUpdateScanProgress()";

    //更新进度条
    ui->progressBarScan->setMaximum(fileCount);
    int progress = ui->progressBarScan->value();
    ui->progressBarScan->setValue(progress+1);

    //更新扫描文件
    ui->labelScanStarting->setText(fileDesc);
    ui->labelScanningFile->setText(currentFile);
}


//处理清理过程中更新UI清理进展信号
void WidgetDiskClean::as_objDiskClean_NotifyUIUpdateCleanProgress(qint64 fileCount,
                                                                  QString fileDesc,
                                                                  QString currentFile)
{
    qDebug() << "处理清理过程中更新UI清理进展信号:on_objDiskClean_NotifyUIUpdateCleanProgress()";
    qDebug() << fileCount << currentFile << fileDesc;

    //更新进度条
    ui->progressBarScan->setMaximum(fileCount);
    int progress = ui->progressBarScan->value();
    ui->progressBarScan->setValue(progress+1);

    //更新扫描文件
    ui->labelScanStarting->setText(fileDesc);
    ui->labelScanningFile->setText(currentFile);

}

//处理完成所有错误扫描信号
void WidgetDiskClean::as_objDiskClean_NotifyUIScanFinished(qint64 iTotalFileErrors,
                                                           double dTotalFileSize,
                                                           qint64 iTotalRegErrors)
{
    qDebug() << "处理完成所有错误扫描信号:on_objDiskClean_NotifyUIScanFinished()";

    m_iTotalFileErrors = iTotalFileErrors;
    m_dTotalFileSize = dTotalFileSize;
    m_iTotalRegErrors = iTotalRegErrors;

    //更新屏幕顶部提示信息
    ui->labelScanStarting->setText(tr("System Scanning has Finished!"));

    //更新进度条
    int max = ui->progressBarScan->maximum();
    ui->progressBarScan->setValue(max);

    //更新表格
    appendTableItem();

    //更新下方屏幕下方统计信息
    QString sTotal = QString("Total %1 trush files or registory error has been found.")
            .arg(m_iTotalFileErrors + m_iTotalRegErrors);
    ui->labelTotalInfo->setText(sTotal);

    enablePushButtons();
    deactivateScanIcon();
}

//处理完成所有错误清理信号
void WidgetDiskClean::as_objDiskClean_NotifyUICleanFinished()
{
    qDebug() << "处理完成所有错误清理信号:on_objDiskClean_NotifyUICleanFinished()";

    //更新屏幕顶部提示信息
    ui->labelScanStarting->setText(tr("System Cleanning has Finished..."));

    //更新表格
    //appendTableItem();

    //更新下方屏幕下方统计信息
    QString total = QString("Total %1 trush files or registory error has been cleaned.")
            .arg(100);
    ui->labelTotalInfo->setText(total);

    enablePushButtons();
    deactivateScanIcon();
}

void WidgetDiskClean::on_pushButtonStartScan_clicked()
{
    qDebug() << "启动一键扫描:on_pushButtonStartScan_clicked()";

    //更新屏幕上方提示信息
    ui->labelScanStarting->setText(tr("Starting to scan your Systerm errors..."));
    ui->labelFilePreInfo->setText(tr("Being Scaned File:"));
    ui->progressBarScan->setValue(0);

    disablePushButtons();
    activateScanIcon();
    cleanTableItem();
}

void WidgetDiskClean::on_pushButtonCleanAll_clicked()
{
    qDebug() << "启动一键清理:on_pushButtonCleanAll_clicked()";

    //更新屏幕上方提示信息
    ui->labelScanStarting->setText(tr("Starting to clean your System errors..."));
    ui->labelFilePreInfo->setText(tr("Being cleaned File:"));
    ui->progressBarScan->setValue(0);

    disablePushButtons();
    activateScanIcon();

}

void WidgetDiskClean::initUI()
{
    ui->labelScanStarting->setText(tr("Press Strat Scanning button to Scan..."));
    ui->labelFilePreInfo->setText(tr(""));
    ui->labelScanningFile->setText(tr(""));
    ui->labelTotalInfo->setText(tr("Total:"));
    ui->pushButtonStartScan->setText(tr("Start Scanning"));
    ui->pushButtonCleanAll->setText(tr("Clean All"));

    //初始化显示结果的表格widget
    ui->tableErrorInfo->setColumnCount(3);
    ui->tableErrorInfo->setSortingEnabled(false);
    ui->tableErrorInfo->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableErrorInfo->verticalHeader()->setHidden(true);
    ui->tableErrorInfo->horizontalHeader()
            ->setStyleSheet("QHeaderView::section{background:skyblue}");
    QStringList headers;
    headers << "Error Type" << "Error Count" << "Total Size(MB)";
    ui->tableErrorInfo->setHorizontalHeaderLabels(headers);
    ui->tableErrorInfo->setColumnWidth(0, TABLE_WIDTH/2-31);
    ui->tableErrorInfo->setColumnWidth(1, TABLE_WIDTH/4);
    ui->tableErrorInfo->setColumnWidth(2, TABLE_WIDTH/4);

    //初始化扫描图片显示label，先显示一张静态的
    deactivateScanIcon();

    //初始化进度条
    ui->progressBarScan->setValue(0);
}

void WidgetDiskClean::appendTableItem()
{
    QTableWidget* table = ui->tableErrorInfo;

    int iRowCount = table->rowCount();
    table->setRowCount(iRowCount+1);
    QTableWidgetItem* item = new QTableWidgetItem();
    QPixmap pixmap(":/disk_clean/file_err");
    item->setIcon(QIcon(pixmap));
    item->setText("Systerm File Errors");
    table->setItem(iRowCount,0,item);

    QTableWidgetItem* itemNum = new QTableWidgetItem();
    itemNum->setText(QString::number(m_iTotalFileErrors));
    table->setItem(iRowCount, 1, itemNum);

    QTableWidgetItem* itemSize = new QTableWidgetItem();
    itemSize->setText(QString::number(m_dTotalFileSize));
    table->setItem(iRowCount, 2, itemSize);

    iRowCount++;

    table->setRowCount(iRowCount+1);
    QTableWidgetItem* item2 = new QTableWidgetItem();
    QPixmap pixmap2(":/disk_clean/reg_err");
    item2->setIcon(QIcon(pixmap2));
    item2->setText("Registory Errors");
    table->setItem(iRowCount,0,item2);

    QTableWidgetItem* itemRegNum = new QTableWidgetItem();
    itemRegNum->setText(QString::number(m_iTotalRegErrors));
    table->setItem(iRowCount, 1, itemRegNum);

    QTableWidgetItem* itemRegSize = new QTableWidgetItem();
    itemRegSize->setText("NA");
    table->setItem(iRowCount, 2, itemRegSize);

    iRowCount++;

}

//清理结果表格
void WidgetDiskClean::cleanTableItem()
{
    QTableWidget* table = ui->tableErrorInfo;
    int iRowCount = table->rowCount();
    if(0 != iRowCount)
    {
        table->clearContents();
        table->setRowCount(0);
    }

}

void WidgetDiskClean::activateScanIcon()
{
    QMovie *movie = new QMovie(":/disk_clean/disk_clean_active");
    ui->labelScanIcon->setMovie(movie);
    movie->start();

}

void WidgetDiskClean::deactivateScanIcon()
{
    QMovie* movie = ui->labelScanIcon->movie();
    if(movie)
    {
        movie->stop();
    }

    QPixmap pixmap(":/disk_clean/disk_clean_static");
    ui->labelScanIcon->setPixmap(pixmap);
}

//扫描or清理时，禁止按钮点击
void WidgetDiskClean::disablePushButtons()
{
    ui->pushButtonStartScan->setDisabled(true);
    ui->pushButtonCleanAll->setDisabled(true);
}

//扫描or清理完成后，启用按钮点击
void WidgetDiskClean::enablePushButtons()
{
    ui->pushButtonStartScan->setEnabled(true);
    ui->pushButtonCleanAll->setEnabled(true);
}



