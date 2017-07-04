#include "objdiskclean.h"
#include "tool.h"
#include "global.h"
#include <QDebug>
#include <QDir>


ObjDiskClean::ObjDiskClean(QObject *parent) :
    QObject(parent)
{
    //m_lstCleanSuffix.push_back("*.~*");
    m_lstCleanSuffix.push_back("ALT");
    m_lstCleanSuffix.push_back("FIX");
    m_lstCleanSuffix.push_back("PRV");
    m_lstCleanSuffix.push_back("SYD");
    m_lstCleanSuffix.push_back("$$$");
    m_lstCleanSuffix.push_back("CLN");
    m_lstCleanSuffix.push_back("GID");
    //m_lstCleanSuffix.push_back("SAV");
    m_lstCleanSuffix.push_back("tmp");
    //m_lstCleanSuffix.push_back("*.?~?");
    m_lstCleanSuffix.push_back("CYP");
    m_lstCleanSuffix.push_back("LOO");
    m_lstCleanSuffix.push_back("SCO");
    m_lstCleanSuffix.push_back("UMB");
    //m_lstCleanSuffix.push_back("*.B?K");
    m_lstCleanSuffix.push_back("ERR");
    //m_lstCleanSuffix.push_back("*.B?K");
    m_lstCleanSuffix.push_back("OLD");
    //m_lstCleanSuffix.push_back("*.B?K");
    m_lstCleanSuffix.push_back("SLK");
    m_lstCleanSuffix.push_back("UBK");
    //m_lstCleanSuffix.push_back("00?");
    m_lstCleanSuffix.push_back("ffa");
    m_lstCleanSuffix.push_back("ffo");
    m_lstCleanSuffix.push_back("ffl");
    m_lstCleanSuffix.push_back("T");
    m_lstCleanSuffix.push_back("ffx");
    m_lstCleanSuffix.push_back("exe");

    m_lstCleanFile.push_back("desktop.ini");
    m_lstCleanFile.push_back("folder.htt");
}

ObjDiskClean::~ObjDiskClean()
{

}

void ObjDiskClean::clean()
{
    clearAllItems();

    UINT typeDev = 0;
    wchar_t ch[4] = { 'X', ':', '\\', '\0' };
    for( wchar_t c = 'A'; c <= 'Z'; c++ )
    {
        ch[0] = c;
        typeDev = ::GetDriveType(ch);
        if( typeDev == DRIVE_FIXED )
        {
            QString strDev = QString::fromWCharArray(ch);
            qDebug()<<"分区："<<strDev<<"开始清理";

            findFiles(strDev);
        }
    }

    //测试文件夹
    //findFiles("C:\\test");

    for(QList<CleanItem*>::iterator it = m_lstCleanItems.begin(); it != m_lstCleanItems.end(); it++)
    {
        qDebug()<<"name:"<<(*it)->m_strName<<"  size:"<<(*it)->m_nTotalSizeKb;
        for(QList<CleanItem*>::iterator it2 = (*it)->m_lstChild.begin(); it2 != (*it)->m_lstChild.end(); it2++)
        {
            qDebug()<<(*it2)->m_strFilePath;
        }
    }

    emit(sigCleanFinish());
}

void ObjDiskClean::findFiles(QString strPath)
{
    QDir dir(strPath);

    if(!dir.exists())
        return;

    dir.setFilter(QDir::Dirs|QDir::Files);

    dir.setSorting(QDir::DirsFirst);

    QFileInfoList list = dir.entryInfoList();

    int i = 0;

    bool bIsDir;

    do{
           QFileInfo fileInfo = list.at(i);

           if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
           {
                i++;
                continue;
           }

            bIsDir = fileInfo.isDir();

           if (bIsDir)
           {
               //fileInfo.size(),fileInfo.fileName(),fileInfo.path()

               findFiles(fileInfo.filePath());
           }
           else
           {
                //qDebug()<<"find file:"<<fileInfo.fileName();

                QString strTmp = fileInfo.absoluteFilePath();
                procFile(strTmp);
           }
           i++;
    }
    while(i<list.size());
}

void ObjDiskClean::addCleanSuffix(QString strSuffix)
{
    if(m_lstCleanSuffix.indexOf(strSuffix.toLower()) != -1)
    {
        m_lstCleanSuffix.push_back(strSuffix.toLower());
    }
}

void ObjDiskClean::removeCleanSuffix(QString strSuffix)
{
    int iIndex = m_lstCleanSuffix.indexOf(strSuffix.toLower());
    if(iIndex != -1)
    {
        m_lstCleanSuffix.removeAt(iIndex);
    }
}

void ObjDiskClean::addCleanFile(QString strFile)
{
    int iIndex = m_lstCleanFile.indexOf(strFile);
    if(iIndex != -1)
    {
        m_lstCleanFile.push_back(strFile);
    }
}

void ObjDiskClean::removeCleanFile(QString strFile)
{
    int iIndex = m_lstCleanFile.indexOf(strFile);
    if(-1 != iIndex)
    {
        m_lstCleanFile.removeAt(iIndex);
    }
}

void ObjDiskClean::procFile(QString strFilePath)
{
    bool bClean = false;
    CleanItem* pItem = NULL;
    QString strCheckSuffix = Tool::getFileSuffix(strFilePath);
    QString strCheckName = Tool::getFileName(strFilePath);

    for(QList<QString>::iterator it = m_lstCleanSuffix.begin(); it != m_lstCleanSuffix.end(); it++)
    {
        qDebug()<<"当前检查的后缀"<<*it;
        qDebug()<<"当前文件的后缀"<<strCheckSuffix;
        if(*it == strCheckSuffix)
        {
            //该文件后缀已经在列表中，添加到其子列表中
            pItem = addCleanItem(0, strCheckSuffix, strFilePath);
            bClean = true;
            break;
        }
    }

    for(QList<QString>::iterator it = m_lstCleanFile.begin(); it != m_lstCleanFile.end(); it++)
    {
        if(*it == strCheckName)
        {
            pItem = addCleanItem(1, strCheckName, strFilePath);
            bClean = true;
            break;
        }
    }

    emit(sigUpdateCurrentCheckFile(bClean, strFilePath, pItem));
}

CleanItem* ObjDiskClean::addCleanItem(int nType, QString strName, QString strFilePath)
{
    CleanItem* pItemRtn = NULL;
    BOOL bExist = FALSE;

    for(QList<CleanItem*>::iterator it = m_lstCleanItems.begin(); it != m_lstCleanItems.end(); it++)
    {
        CleanItem* pItem = *it;
        if((pItem->m_nType == nType) && (pItem->m_strName == strName))
        {
            bExist = TRUE;
            CleanItem* pItemNew = new CleanItem();
            pItemNew->m_nType = nType;
            pItemNew->m_strName = strName;
            pItemNew->m_strFilePath = strFilePath;
            pItemNew->m_nTotalSizeKb = Tool::getFileSize(strFilePath);

            pItem->m_lstChild.push_back(pItem);
            pItem->m_nTotalSizeKb += pItemNew->m_nTotalSizeKb;

            pItemRtn = pItemNew;

            break;
        }
    }

    if(!bExist)
    {
        CleanItem* pItemChild = new CleanItem();
        pItemChild->m_nType = 1;
        pItemChild->m_strName = strName;
        pItemChild->m_strFilePath = strFilePath;
        pItemChild->m_nTotalSizeKb = Tool::getFileSize(strFilePath);

        CleanItem* pItemNew = new CleanItem();
        pItemNew->m_nType = 0;
        pItemNew->m_strName = strName;
        pItemNew->m_nTotalSizeKb = pItemChild->m_nTotalSizeKb;
        pItemNew->m_lstChild.push_back(pItemChild);

        m_lstCleanItems.push_back(pItemNew);

        pItemRtn = pItemChild;
    }
}

void ObjDiskClean::clearAllItems()
{
    for(QList<CleanItem*>::iterator it = m_lstCleanItems.begin(); it != m_lstCleanItems.end(); it++)
    {
        CleanItem* pTmp = *it;
        delete pTmp;
        pTmp = NULL;
    }
    m_lstCleanItems.clear();
}

