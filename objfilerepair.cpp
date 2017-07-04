#include "objfilerepair.h"
#include "tool.h"
#include "global.h"
#include <QFileInfo>
#include <QMessageBox>

ObjFileRepair::ObjFileRepair(QObject *parent) :
    QObject(parent)
{
    m_pHttp = new Http(this);




    connect( m_pHttp, &Http::sigCurrentDownloadFile, this, &ObjFileRepair::CurrentDownloadFile );
    connect( m_pHttp, &Http::sigDownloadFinished, this, &ObjFileRepair::downloadFinished );
}

ObjFileRepair::~ObjFileRepair()
{
    FileListDownload* p = NULL;
    for( int i = 0; i < m_lstFiles.count(); i++ )
    {
        p = m_lstFiles.at(i);
        delete p;
        p = NULL;
    }
}

void ObjFileRepair::CurrentDownloadFile( int nPercentage )
{
    emit sigUpdateDownload(nPercentage);
}

void ObjFileRepair::downloadFile( QString strFile )
{
    //QString strPath;
    //strPath = g_strDownloadPath + "\\" + strFile;
    //strPath.replace( ".", "_" );

    if(strFile.isEmpty())
    {
        return;
    }

    //如果有文件，先删除
    QString strFilePath = m_strFilePathRepair + "\\" + strFile;
    if(Tool::isFileOrDirExist(strFilePath))
    {
        Tool::reomveDir(strFilePath);
    }

    m_strFileDownloadName = strFile;

    if(!Tool::isFileOrDirExist(Global::s_strDownloadPath))
    {
        //创建文件夹
        Tool::createDirectory( Global::s_strDownloadPath, false );
    }
    else
    {

    }

    QString strUrl = Global::s_strHttpUrl;
    QString strSuffix = Tool::getFileSuffix(strFile);
    //QString strFileName = Tool::getFileName();
    strSuffix.toLower();
    if( "dll" == strSuffix )
    {
        strUrl += "/dll/";
        strUrl += strFile.at(0);
        strUrl += "/";
    }
    else
    {
        strUrl += "/";
        strUrl += strSuffix;
        strUrl += "/";
    }
    QString strName = Tool::getFileNameWithoutSuffix(strFile);
    strUrl += strName;
    strUrl += ".rar";

    QString strDownload = Global::s_strDownloadPath;
    strDownload += "\\";
    strDownload += strName;
    strDownload += ".rar";

    //QString strUrlTmp = getFileRepairUrl(strFile);
    Global::saveIniData( "LastRepairFile", strFile );
    m_pHttp->downloadFile( strUrl, strDownload );
}

void ObjFileRepair::downloadFinished( QString strFile, bool bSuccess )
{
    qDebug()<<"ObjFileRepair::downloadFinished begin";

    FileListDownload* pDownload = new FileListDownload();
    pDownload->m_strFileName = m_strFileDownloadName;

    if(!bSuccess)
    {
        //QMessageBox::about( NULL, "Global::s_strExeName", "not found" );
        //QMessageBox::about( this, "s_strExeName", "not found" );

        pDownload->m_strError = strFile;
        emit sigDownloadFinished(pDownload, false);

        qDebug()<<"ObjFileRepair::downloadFinished end";
        return;
    }

    //QString strName = Tool::getFileNameWithoutSuffix(strFile);
    QString strSaveTmp = m_strFilePathRepair;
    QString strSave;
    strSaveTmp += "\\";
    strSaveTmp += m_strFileDownloadName;
    strSave = strSaveTmp;
    strSaveTmp += "tmp";

    if(!Tool::isFileOrDirExist(strSaveTmp))
    {
        //创建文件夹
        Tool::createDirectory( strSaveTmp, false );
    }

    if(!Tool::isFileOrDirExist(strSave))
    {
        //创建文件夹
        Tool::createDirectory( strSave, false );
    }

    QString str7z = Global::s_strCurrentPath + "\\" + "7z.exe";
    Tool::uncompressFile( strFile, strSaveTmp, str7z );
    //Tool::openAndSelectFile(strSaveTmp);

    qDebug()<<"ObjFileRepair::downloadFinished 2";

    //重新分配文件
    pDownload->m_strPath = strSave;
    pDownload->generateVersionList(strSaveTmp);

    qDebug()<<"ObjFileRepair::downloadFinished 3";

    //删除原文件
    Tool::removeFilesinDir(strSaveTmp);
    //清空FileDownload目录
    Tool::reomveDir(Global::s_strDownloadPath);

    m_lstFiles.push_back(pDownload);

    qDebug()<<"ObjFileRepair::downloadFinished 4";

    //删除压缩文件
    Tool::deleteFile(strFile);
    m_strFileDownloadName.clear();

    qDebug()<<"ObjFileRepair::downloadFinished 5";

    emit sigDownloadFinished(pDownload, true);

    qDebug()<<"ObjFileRepair::downloadFinished end";

    Global::saveIniData( "LastRepairFile", "" );
}

void ObjFileRepair::openFile( int nIndex )
{
    m_lstFiles[nIndex]->open();
}

void ObjFileRepair::deleteDownloadFile(int nIndex)
{
    m_lstFiles.removeAt(nIndex);
}

void ObjFileRepair::init()
{
    m_strFilePathRepair = QString("%1\\FileRepair").arg(Global::s_strCurrentPath);
    m_strFilePathRepair.replace( "/", "\\" );

    //读取已下载的文件
    QDir dir(m_strFilePathRepair);
    if(!dir.exists())
    {
        return;
    }

    QDirIterator it( m_strFilePathRepair, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot/*, QDirIterator::Subdirectories*/ );
    while(it.hasNext())
    {
        it.next();
        QFileInfo info = it.fileInfo();

        FileListDownload* pFileDownload = new FileListDownload();
        pFileDownload->m_strFileName = info.fileName();
        pFileDownload->m_strPath = m_strFilePathRepair + "\\" + pFileDownload->m_strFileName;
        if(pFileDownload->loadVersionList())
        {
            emit sigDownloadFinished( pFileDownload, true );
        }
        else
        {
            delete pFileDownload;
            pFileDownload = NULL;
        }
    }

    //继续上次下载的文件

}

void ObjFileRepair::timerEvent(QTimerEvent *event)
{
    m_pHttp->cancelDownload();
}

QString ObjFileRepair::getFileRepairUrl(QString strFile)
{
    QString strUrl = Global::s_strHttpUrl;
    QString strSuffix = Tool::getFileSuffix(strFile);
    //QString strFileName = Tool::getFileName();
    strSuffix.toLower();
    if( "dll" == strSuffix )
    {
        strUrl += "/dll/";
        strUrl += strFile.at(0);
        strUrl += "/";
    }
    else
    {
        strUrl += "/";
        strUrl += strSuffix;
        strUrl += "/";
    }
    QString strName = Tool::getFileNameWithoutSuffix(strFile);
    strUrl += strName;
    strUrl += ".rar";

    return strUrl;
}

QString ObjFileRepair::getDownloadFilePath(QString strName)
{
    QString strDownload = Global::s_strDownloadPath;
    strDownload += "\\";
    strDownload += strName;
    strDownload += ".rar";

    return strDownload;
}
