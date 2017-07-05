#include "widgetdiskclean.h"

#include "ui_widgetdiskclean.h"
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "global.h"

WidgetDiskClean::WidgetDiskClean(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WidgetDiskClean)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(3);
    QStringList headers;
    headers << "suffix" << "name" << "size" << "path";
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setColumnWidth(0, 70);
    ui->treeWidget->setColumnWidth(1, 220);
    ui->treeWidget->setColumnWidth(2, 40);
    ui->treeWidget->setColumnWidth(3, 350);


    m_pObjDiskClean = new ObjDiskClean();
    m_pThread = new QThread();
    m_pObjDiskClean->moveToThread(m_pThread);
    m_pThread->start();

    connect(ui->pushButton_disk_clean, &QPushButton::clicked, m_pObjDiskClean, &ObjDiskClean::clean);
    connect(m_pObjDiskClean, &ObjDiskClean::sigUpdateCurrentCheckFile, this, &WidgetDiskClean::updateCurrentCheckFile,
            Qt::QueuedConnection);
    connect(m_pObjDiskClean, &ObjDiskClean::sigCleanFinish, this, &WidgetDiskClean::cleanFinish, Qt::QueuedConnection);
    connect(this, &WidgetDiskClean::sigClean, m_pObjDiskClean, &ObjDiskClean::clean, Qt::BlockingQueuedConnection);
}

WidgetDiskClean::~WidgetDiskClean()
{
    delete m_pObjDiskClean;
    m_pObjDiskClean = NULL;
}

void WidgetDiskClean::updateCurrentCheckFile(bool bClean, QString strFilePath, CleanItem* pItem)
{
    ui->label_diskclean_file->setText(strFilePath);

    if(!bClean)
    {
        return;
    }


    QList<QTreeWidgetItem*> lst = ui->treeWidget->findItems(pItem->m_strName, Qt::MatchFixedString);
    QTreeWidgetItem* pTreeItem = NULL;
    if(0 == lst.size())
    {
        //添加新的父节点
        QStringList title;
        title << pItem->m_strName;
        pTreeItem = new QTreeWidgetItem(ui->treeWidget, title);
        QStringList list;
        list << "" << "" << "";
        QTreeWidgetItem* leaf = new QTreeWidgetItem(pTreeItem, list);
        pTreeItem->addChild(leaf);
    }
    else if(1 == lst.size())
    {
        //添加到父节点下
        pTreeItem = lst.at(0);
        QStringList list1;
        list1 << "" << pItem->m_strName << QString::number(pItem->m_nTotalSizeKb) << pItem->m_strFilePath;
        QTreeWidgetItem* leaf1 = new QTreeWidgetItem(pTreeItem, list1);
        pTreeItem->addChild(leaf1);

    }
    else
    {
        //有多个节点，异常状态，提示错误
        QString strMsg = QString("%s 有多个").arg(pItem->m_strName);
        QMessageBox::about(this, Global::s_strExeName, strMsg);
    }
}

void WidgetDiskClean::cleanFinish()
{
    qDebug() << "WidgetDiskClean::cleanFinish";


}
