#include "objdiskclean.h"
#include "tool.h"
#include "global.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QSettings>
#include <QList>
#include <QProcessEnvironment>

ObjDiskClean::ObjDiskClean(QObject* parent) :
    QObject(parent),
    m_iTotalScanedFiles(0),
    m_iTotalScanedRegs(0)
{
    m_pDiskScanConfig = CDiskScanConfig::loadConfig();
//    //m_lstTrashFileSuffix.push_back("*.~*");
//    m_lstTrashFileSuffix.push_back("ALT");
//    m_lstTrashFileSuffix.push_back("FIX");
//    m_lstTrashFileSuffix.push_back("PRV");
//    m_lstTrashFileSuffix.push_back("SYD");
//    m_lstTrashFileSuffix.push_back("$$$");
//    m_lstTrashFileSuffix.push_back("CLN");
//    m_lstTrashFileSuffix.push_back("GID");
//    //m_lstTrashFileSuffix.push_back("SAV");
//    m_lstTrashFileSuffix.push_back("tmp");
//    //m_lstTrashFileSuffix.push_back("*.?~?");
//    m_lstTrashFileSuffix.push_back("CYP");
//    m_lstTrashFileSuffix.push_back("LOO");
//    m_lstTrashFileSuffix.push_back("SCO");
//    m_lstTrashFileSuffix.push_back("UMB");
//    //m_lstTrashFileSuffix.push_back("*.B?K");
//    m_lstTrashFileSuffix.push_back("ERR");
//    //m_lstTrashFileSuffix.push_back("*.B?K");
//    m_lstTrashFileSuffix.push_back("OLD");
//    //m_lstTrashFileSuffix.push_back("*.B?K");
//    m_lstTrashFileSuffix.push_back("SLK");
//    m_lstTrashFileSuffix.push_back("UBK");
//    //m_lstTrashFileSuffix.push_back("00?");
//    m_lstTrashFileSuffix.push_back("ffa");
//    m_lstTrashFileSuffix.push_back("ffo");
//    m_lstTrashFileSuffix.push_back("ffl");
//    m_lstTrashFileSuffix.push_back("T");
//    m_lstTrashFileSuffix.push_back("ffx");
//    m_lstTrashFileSuffix.push_back("exe");
//
//    m_lstNeedToBeCleanedFiles.push_back("desktop.ini");
//    m_lstNeedToBeCleanedFiles.push_back("folder.htt");
    
}

ObjDiskClean::~ObjDiskClean()
{
    if(NULL != m_pDiskScanConfig)
    {
        delete m_pDiskScanConfig;
        m_pDiskScanConfig = NULL;
    }
}

//扫描所有注册表错误和磁盘垃圾
void ObjDiskClean::scanAllErrors()
{
    qDebug() << "扫描所有注册表错误和磁盘垃圾:scanAllErrors()";

    m_iTotalScanedFiles = 0;
    m_iTotalScanedRegs = 0;
    scanAllTrashFiles();
    scanAllRegErrors();

    //完成所有扫描后通知UI
    emit sigNotifyUIScanFinished(m_iTotalScanedFiles, m_iTotalScanedRegs);
}

//清理所有注册表错误和磁盘垃圾
void ObjDiskClean::cleanAllErrors()
{
    qDebug() << "清理所有注册表错误和磁盘垃圾:cleanAllErrors()";

    cleanAllTrashFiles();
    cleanAllRegErrors();

    //完成所有清理后通知UI
    emit sigNotifyUICleanFinished();
}

void ObjDiskClean::scanAllTrashFiles()
{
    qDebug() << "扫描所有磁盘垃圾:scanAllTrashFiles()";

    //获取需要扫描的路径信息列表
    QList<TDiskScanInfo*>* pList = m_pDiskScanConfig->getDiskScanInfo();
    if(NULL != pList)
    {
        QListIterator<TDiskScanInfo*> it(*pList);
        while(it.hasNext())
        {
            scanTrashFilesInPath(it.next());
        }
    }
}

void ObjDiskClean::scanAllRegErrors()
{
    m_iTotalScanedRegs = 1000;
}

void ObjDiskClean::cleanAllTrashFiles()
{

}

void ObjDiskClean::cleanAllRegErrors()
{

}

void ObjDiskClean::scanTrashFilesInPath(TDiskScanInfo* scanInfo)
{
    qDebug() << "清理指定目录的磁盘垃圾:scanTrashFilesInPath()";
    qDebug() << "正在扫描" << scanInfo->desc_cn;
    qDebug() << scanInfo->trash_path;

    scanInfo->total_size = 0;
    scanInfo->file_count = 0;
    scanInfo->fileList.clear();

    //不用递归，构建一个目录扫描列表进行遍历
    QStringList scanDirList;
    scanDirList.append(scanInfo->trash_path);

    QFileInfo fileInfo;
    QDir scanDir;
    QStringList extNameList;
    QFileInfoList scanDirFiles;
    QString srcDir;


    for(int i = 0; i < scanDirList.count(); i++)
    {
        srcDir = scanDirList.at(i);
        replaceEnv(srcDir);
        scanDir.setPath(srcDir);
        if(!scanDir.exists())
        {
            continue;
        }
        scanDir.setFilter(QDir::Dirs|QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot|QDir::Hidden|QDir::System);
//        if(scanDir.isEmpty())
//        {
//            continue;
//        }

        //目录时按照文件通配符筛选垃圾文件
        if(0 == scanInfo->clean_all_flag)
        {
            extNameList = scanInfo->ext_name.split(";");
            scanDir.setNameFilters(extNameList);
        }

        scanDirFiles = scanDir.entryInfoList();
        QListIterator<QFileInfo> itFile(scanDirFiles);
        while(itFile.hasNext())
        {
            fileInfo = itFile.next();
            if(fileInfo.isFile())
            {
                scanInfo->total_size += fileInfo.size()/1024/1024; //转换成MB
                scanInfo->file_count++;
                m_iTotalScanedFiles++;
                scanInfo->fileList.append(fileInfo.filePath());
                emit sigNotifyUIUpdateScanProgress(scanDir.count(), scanInfo->desc_en, fileInfo.filePath());
            }
            else
            {
                //如果时目录，就继续加到待扫描的目录列表中
                scanDirList.append(fileInfo.filePath());
            }
        }
    }
}

void ObjDiskClean::replaceEnv(QString& srcDir)
{
    if(srcDir.contains("%"))
    {
        int begin = srcDir.indexOf("%");
        int end = srcDir.lastIndexOf("%");
        QString env = srcDir.mid(begin + 1, end - begin - 1);
        //qDebug() << QProcessEnvironment::systemEnvironment().toStringList();
        QString path = QProcessEnvironment::systemEnvironment().value(env);
        srcDir.replace(begin, (end - begin + 1), path);
    }
}



//void ObjDiskClean::clean()
//{
//    clearAllItems();
//
//    UINT typeDev = 0;
//    wchar_t ch[4] = { 'X', ':', '\\', '\0' };
//    for(wchar_t c = 'A'; c <= 'Z'; c++)
//    {
//        ch[0] = c;
//        typeDev = ::GetDriveType(ch);
//        if(typeDev == DRIVE_FIXED)
//        {
//            QString strDev = QString::fromWCharArray(ch);
//            qDebug() << "分区：" << strDev << "开始清理";
//
//            findFiles(strDev);
//        }
//    }
//
//    //测试文件夹
//    //findFiles("C:\\test");
//
//    for(QList<CleanItem*>::iterator it = m_lstNeedToBeCleanedItems.begin(); it != m_lstNeedToBeCleanedItems.end(); it++)
//    {
//        qDebug() << "name:" << (*it)->m_strName << "  size:" << (*it)->m_nTotalSizeKb;
//        for(QList<CleanItem*>::iterator it2 = (*it)->m_lstChild.begin(); it2 != (*it)->m_lstChild.end(); it2++)
//        {
//            qDebug() << (*it2)->m_strFilePath;
//        }
//    }
//
//    emit(sigCleanFinish());
//}
//
//void ObjDiskClean::findFiles(QString strPath)
//{
//    QDir dir(strPath);
//
//    if(!dir.exists())
//    { return; }
//
//    dir.setFilter(QDir::Dirs | QDir::Files);
//
//    dir.setSorting(QDir::DirsFirst);
//
//    QFileInfoList list = dir.entryInfoList();
//
//    int i = 0;
//
//    bool bIsDir;
//
//    do
//    {
//        QFileInfo fileInfo = list.at(i);
//
//        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
//        {
//            i++;
//            continue;
//        }
//
//        bIsDir = fileInfo.isDir();
//
//        if(bIsDir)
//        {
//            //fileInfo.size(),fileInfo.fileName(),fileInfo.path()
//
//            findFiles(fileInfo.filePath());
//        }
//        else
//        {
//            //qDebug()<<"find file:"<<fileInfo.fileName();
//
//            QString strTmp = fileInfo.absoluteFilePath();
//            procFile(strTmp);
//        }
//        i++;
//    }
//    while(i < list.size());
//}
//
//void ObjDiskClean::addCleanSuffix(QString strSuffix)
//{
//    if(m_lstTrashFileSuffix.indexOf(strSuffix.toLower()) != -1)
//    {
//        m_lstTrashFileSuffix.push_back(strSuffix.toLower());
//    }
//}
//
//void ObjDiskClean::removeCleanSuffix(QString strSuffix)
//{
//    int iIndex = m_lstTrashFileSuffix.indexOf(strSuffix.toLower());
//    if(iIndex != -1)
//    {
//        m_lstTrashFileSuffix.removeAt(iIndex);
//    }
//}
//
//void ObjDiskClean::addCleanFile(QString strFile)
//{
//    int iIndex = m_lstNeedToBeCleanedFiles.indexOf(strFile);
//    if(iIndex != -1)
//    {
//        m_lstNeedToBeCleanedFiles.push_back(strFile);
//    }
//}
//
//void ObjDiskClean::removeCleanFile(QString strFile)
//{
//    int iIndex = m_lstNeedToBeCleanedFiles.indexOf(strFile);
//    if(-1 != iIndex)
//    {
//        m_lstNeedToBeCleanedFiles.removeAt(iIndex);
//    }
//}
//
//void ObjDiskClean::procFile(QString strFilePath)
//{
//    bool bClean = false;
//    CleanItem* pItem = NULL;
//    QString strCheckSuffix = Tool::getFileSuffix(strFilePath);
//    QString strCheckName = Tool::getFileName(strFilePath);
//
//    for(QList<QString>::iterator it = m_lstTrashFileSuffix.begin(); it != m_lstTrashFileSuffix.end(); it++)
//    {
//        qDebug() << "当前检查的后缀" << *it;
//        qDebug() << "当前文件的后缀" << strCheckSuffix;
//        if(*it == strCheckSuffix)
//        {
//            //该文件后缀已经在列表中，添加到其子列表中
//            pItem = addCleanItem(0, strCheckSuffix, strFilePath);
//            bClean = true;
//            break;
//        }
//    }
//
//    for(QList<QString>::iterator it = m_lstNeedToBeCleanedFiles.begin(); it != m_lstNeedToBeCleanedFiles.end(); it++)
//    {
//        if(*it == strCheckName)
//        {
//            pItem = addCleanItem(1, strCheckName, strFilePath);
//            bClean = true;
//            break;
//        }
//    }
//
//    emit(sigUpdateCurrentCheckFile(bClean, strFilePath, pItem));
//}
//
//CleanItem* ObjDiskClean::addCleanItem(int nType, QString strName, QString strFilePath)
//{
//    CleanItem* pItemRtn = NULL;
//    BOOL bExist = FALSE;
//
//    for(QList<CleanItem*>::iterator it = m_lstNeedToBeCleanedItems.begin(); it != m_lstNeedToBeCleanedItems.end(); it++)
//    {
//        CleanItem* pItem = *it;
//        if((pItem->m_nType == nType) && (pItem->m_strName == strName))
//        {
//            bExist = TRUE;
//            CleanItem* pItemNew = new CleanItem();
//            pItemNew->m_nType = nType;
//            pItemNew->m_strName = strName;
//            pItemNew->m_strFilePath = strFilePath;
//            pItemNew->m_nTotalSizeKb = Tool::getFileSize(strFilePath);
//
//            pItem->m_lstChild.push_back(pItem);
//            pItem->m_nTotalSizeKb += pItemNew->m_nTotalSizeKb;
//
//            pItemRtn = pItemNew;
//
//            break;
//        }
//    }
//
//    if(!bExist)
//    {
//        CleanItem* pItemChild = new CleanItem();
//        pItemChild->m_nType = 1;
//        pItemChild->m_strName = strName;
//        pItemChild->m_strFilePath = strFilePath;
//        pItemChild->m_nTotalSizeKb = Tool::getFileSize(strFilePath);
//
//        CleanItem* pItemNew = new CleanItem();
//        pItemNew->m_nType = 0;
//        pItemNew->m_strName = strName;
//        pItemNew->m_nTotalSizeKb = pItemChild->m_nTotalSizeKb;
//        pItemNew->m_lstChild.push_back(pItemChild);
//
//        m_lstNeedToBeCleanedItems.push_back(pItemNew);
//
//        pItemRtn = pItemChild;
//    }
//}
//
//void ObjDiskClean::clearAllItems()
//{
//    for(QList<CleanItem*>::iterator it = m_lstNeedToBeCleanedItems.begin(); it != m_lstNeedToBeCleanedItems.end(); it++)
//    {
//        CleanItem* pTmp = *it;
//        delete pTmp;
//        pTmp = NULL;
//    }
//    m_lstNeedToBeCleanedItems.clear();
//}

