#include "filelistitem.h"
#include "global.h"
#include "tool.h"

#include <QMessageBox>

QString FileListItem::s_strFilePathSysScan;

FileListItem::FileListItem(QObject *parent) :
    QObject(parent)
{
    m_pHttp = new Http;

    m_eStatus = E_STATUS_DISPLAY;

    m_pBtnDownload = NULL;
    m_pBtnDelete = NULL;

    FileListItem::s_strFilePathSysScan = QString("%1\\SysScan\\").arg(Global::s_strCurrentPath);
    FileListItem::s_strFilePathSysScan.replace( "/", "\\" );

    m_nIndex = -1;
}

FileListItem::~FileListItem()
{
    if(m_pHttp)
    {
        delete m_pHttp;
        m_pHttp = NULL;
    }
}

void FileListItem::downloadFile()
{


    m_eStatus = E_STATUS_DOWNLOADING;

    connect( m_pHttp, &Http::sigCurrentDownloadFile, this, &FileListItem::CurrentDownloadFile );
    connect( m_pHttp, &Http::sigDownloadFinished, this, &FileListItem::downloadFinished );

    int nIndex1 = m_strFilePath.indexOf('\\');
    int nIndex2 = m_strFilePath.lastIndexOf('\\');
    QString strTmp = m_strFilePath.mid( nIndex1+1, nIndex2 - nIndex1 );

    m_strFileDownload = FileListItem::s_strFilePathSysScan + strTmp + m_strFileName;
    {
        QString strTmp;
        strTmp = m_strFilePath.right( m_strFilePath.count() - 11 );
        strTmp.replace( "\\", "/" );
        m_strFileUrl = Global::s_strSystemUrl + strTmp;
    }
    //m_strFileUrl += "1";
    m_pHttp->downloadFile( m_strFileUrl, m_strFileDownload );

    qDebug()<<"FileListItem::downloadFile() thread:"<<QThread::currentThreadId();
}

void FileListItem::cancelDownload()
{
    m_eStatus = E_STATUS_DISPLAY;
    m_pHttp->cancelDownload();
}

void FileListItem::setThread( QThread* pThread )
{
    m_pHttp->moveToThread(pThread);
}

void FileListItem::setDownloadButton( QPushButton* pBtn )
{
    m_pBtnDownload = pBtn;
}

void FileListItem::setDeleteButton( QPushButton* pBtn )
{
    m_pBtnDelete = pBtn;
}

void FileListItem::CurrentDownloadFile( int nPercentage )
{
//    if( 0 == nPercentage )
//    {
//        m_pPbr->setRange( 0, 100 );
//    }
    //m_pPbr->setValue(nPercentage);

    //qDebug()<<"FileListItem::CurrentDownloadFile:"<<QThread::currentThreadId();
    sigCurrentDownloadFile( this, nPercentage );
}

void FileListItem::openFile()
{
    //QMessageBox::about( NULL, m_strFileDownload, m_strFileDownload);
    Tool::openAndSelectFile(m_strFileDownload);
}

void FileListItem::downloadFinished( QString strFile, bool bSuccess )
{
    qDebug()<<strFile<<"download finished, bSuccess = "<<bSuccess;
    if(bSuccess)
    {
        m_eStatus = E_STATUS_LOCAL;
    }
    else
    {
        m_strDownloadError = strFile;
        m_eStatus = E_STATUS_DISPLAY;
        Tool::deleteFile(m_strFileDownload);
    }

    emit sigDownloadFinished( this, bSuccess );
}
