#include "filelistdownload.h"
#include "widgetversion.h"
#include "global.h"
#include "Tool.h"
#include <QDebug>
#include <QDir>
#include <QDirIterator>

FileListDownload::FileListDownload(QObject *parent) :
    QObject(parent)
{

}

void FileListDownload::deleteFile()
{

}

void FileListDownload::addVersionItem( QVector<VersionListItem*>& vct, VersionListItem* pItem )
{
    for( int i = 0; i < vct.size(); i++ )
    {
        QString strFilePath;
        if(pItem->m_strFileVersion.isEmpty())
        {
            if( vct[i]->m_fSizeKb <= pItem->m_fSizeKb )
            {
                vct.insert( i, pItem );
                return;
            }
        }
        else
        {
            if( VersionListItem::isGreaterVersion( pItem->m_strFileVersion, vct[i]->m_strFileVersion ) )
            {
                vct.insert( i, pItem );
                return;
            }
        }


    }

    vct.push_back(pItem);
}

void FileListDownload::generateVersionList( QString strPathTmp )
{
    QDir dir(strPathTmp);
    if(!dir.exists())
    {
        return;
    }

    bool bOneVersion = false;

    QDirIterator it( strPathTmp, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories );
    while(it.hasNext())
    {
        it.next();
        QFileInfo info = it.fileInfo();

        if( info.fileName() == "DLL工具.exe" )
        {
            continue;
        }

        QString strPathOrg = info.absoluteFilePath();
        QString strFileVersion = Tool::getFileVersion(strPathOrg);
        float fSizeKb = Tool::getFileSize(strPathOrg);
        if( fSizeKb == 0 )
        {
            continue;
        }

        VersionListItem* pItem = new VersionListItem;
        pItem->m_strFileVersion = strFileVersion;
        pItem->m_fSizeKb = fSizeKb;
        pItem->m_strFilePath = m_strPath + "\\";

        Tool::isFileTypeX86(strPathOrg);
        if(Tool::isFileTypeX86(strPathOrg))
        {
            pItem->m_bX86 = true;
            pItem->m_strFilePath += "x86\\";

            addVersionItem( m_vctVersionX86, pItem );
        }
        else
        {
            pItem->m_bX86 = false;
            pItem->m_strFilePath += "x64\\";

            addVersionItem( m_vctVersionX64, pItem );
        }



        //如果能获取版本号，则用版本号做文件夹名 v_1.2.0.0
        if(!pItem->m_strFileVersion.isEmpty())
        {
            pItem->m_strFilePath += "v_";
            pItem->m_strFilePath += pItem->m_strFileVersion;
        }
        else  //用大小(kb)做文件夹名
        {
            pItem->m_strFilePath += "s_";
            QString strTmp = QString("%1").arg(pItem->m_fSizeKb);
            pItem->m_strFilePath += strTmp;
        }

        pItem->m_strFilePath += "\\";
        pItem->m_strFilePath += m_strFileName;
        Tool::createDirectory( pItem->m_strFilePath, true );

        //如果没有注册，只保留一个文件
        if(Global::s_bActive)
        { 
            if(!QFile::copy( strPathOrg, pItem->m_strFilePath ))
            {
                QString strTmp = QString("copy %1 to %2 failed").arg(strPathOrg).arg(pItem->m_strFilePath);
                qDebug()<<strTmp;
            }

            {
                pItem->m_bEmpty = false;
            }
        }
        else
        {
            if(!bOneVersion)
            {
                if(!QFile::copy( strPathOrg, pItem->m_strFilePath ))
                {
                    QString strTmp = QString("copy %1 to %2 failed").arg(strPathOrg).arg(pItem->m_strFilePath);
                    qDebug()<<strTmp;
                }

                {
                    pItem->m_bEmpty = false;
                }
                bOneVersion = true;
            }
            else
            {
                QFile file(pItem->m_strFilePath);
                file.open(QIODevice::WriteOnly);
                file.close();

                pItem->m_bEmpty = true;
            }
        }
    }
}

bool FileListDownload::loadVersionList()
{
//    QDirIterator it( m_strPath, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot/*, QDirIterator::Subdirectories*/ );
//    while(it.hasNext())
//    {
//        it.next();
//        QFileInfo info = it.fileInfo();

//        if( info.isDir() && info.fileName() == "x86" )
//        {
//            int n = 0;
//        }
//        else if( info.isDir() && info.fileName() == "x64" )
//        {
//            int n = 0;
//        }
//    }
    bool bHas = false;

    QDirIterator it( m_strPath, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories );
    while(it.hasNext())
    {
        bHas = true;

        it.next();
        QFileInfo info = it.fileInfo();

        QString strPathOrg = info.absoluteFilePath();
        QString strFileVersion = Tool::getFileVersion(strPathOrg);
        float fSizeKb = Tool::getFileSize(strPathOrg);

        VersionListItem* pItem = new VersionListItem;
        pItem->m_strFileVersion = strFileVersion;
        pItem->m_fSizeKb = fSizeKb;
        pItem->m_strFilePath = m_strPath + "\\";

        if( 0 == fSizeKb )
        {
            pItem->m_bEmpty = true;
        }
        else
        {
            pItem->m_bEmpty = false;
        }

        Tool::isFileTypeX86(strPathOrg);
        if(Tool::isFileTypeX86(strPathOrg))
        {
            pItem->m_bX86 = true;
            pItem->m_strFilePath += "x86\\";

            addVersionItem( m_vctVersionX86, pItem );
        }
        else
        {
            pItem->m_bX86 = false;
            pItem->m_strFilePath += "x64\\";

            addVersionItem( m_vctVersionX64, pItem );
        }

        //如果能获取版本号，则用版本号做文件夹名 v_1.2.0.0
        if(!pItem->m_strFileVersion.isEmpty())
        {
            pItem->m_strFilePath += "v_";
            pItem->m_strFilePath += pItem->m_strFileVersion;
        }
        else  //用大小(kb)做文件夹名
        {
            pItem->m_strFilePath += "s_";
            QString strTmp = QString("%1").arg(pItem->m_fSizeKb);
            pItem->m_strFilePath += strTmp;
        }

        pItem->m_strFilePath += "\\";
        pItem->m_strFilePath += m_strFileName;

    }

    return bHas;
}

void FileListDownload::open()
{
    WidgetVersion widgetVersion(this);
    widgetVersion.exec();
}
