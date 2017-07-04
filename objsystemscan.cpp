#include "objsystemscan.h"
#include "Tool.h"
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QDir>
#include <QDebug>
#include <QDirIterator>

#include <windows.h>



ObjSystemScan::ObjSystemScan(QObject *parent) :
    QObject(parent)
{
    m_strDatfile = "win7_x64_ultimate.txt";
    //m_strDatfile = "C:\\win7_x64_ultimate.txt";

    m_bChecked = false;
    m_bDownload = false;

    m_nFixedCount = 0;
    m_nCurIndex = 0;

//    //读取dat文件
//    ReadDatFile();


}

ObjSystemScan::~ObjSystemScan()
{
    FileListItem* pItem = NULL;
    QMapIterator<QString, CommonFiles> it(m_mapFile);
    while (it.hasNext()) {
        it.next();
        for( int j = 0; j < it.value().lstFiles.size(); j++ )
        {
            pItem = it.value().lstFiles.at(j);

//            if( pItem->m_eStatus == E_STATUS_LOCAL )
//            {
//                nFixedCount++;
//            }

            delete pItem;
            pItem = NULL;
        }
    }



}

void ObjSystemScan::startCheckDlls()
{
    if(m_bChecked)
    {
        return;
    }

    qDebug()<<"system scan thread:"<<QThread::currentThreadId();

    //开始检查
    CheckSystemFile();
}

void ObjSystemScan::pauseCheckDlls()
{

}

void ObjSystemScan::quitCheckDlls()
{

}

void ObjSystemScan::ReadDatFile()
{
    TCHAR sysDir[128];
    for( int i = 0; i < 128; i++ )
    {
        sysDir[i] = 0;
    }

    GetSystemDirectory( sysDir, 128 * sizeof(TCHAR));
    QString strSysDir = QString::fromWCharArray(sysDir);
    strSysDir = strSysDir.left(11);

    QString strDatPath = Global::s_strCurrentPath + "\\" + Global::s_strDatFile;
    QFile file(strDatPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"open file "<<strDatPath<<" failed,error:"<<file.errorString();
    }
    else
    {

    }

    QTextStream txtInput(&file);
    QString strLine;
    QString strFileDir;
    QString strFilePath;
    while(!txtInput.atEnd())
    {
        strLine = txtInput.readLine();
        if( "[" == strLine.left(1) )
        {
            int nBgn = strLine.indexOf("[");
            int nEnd = strLine.indexOf("]");
            QString strTmp = strLine.mid( nBgn + 1,  nEnd - nBgn - 1  );
            strFileDir = strSysDir + strTmp + "\\";
        }
        else
        {
            if(!isFileNotCheck(strLine))
            {
                strFilePath = strFileDir + strLine;
                //qDebug()<<"file " + strFilePath + " append";
                m_lstFileTotal.append(strFilePath);
            }

        }
    }
    file.close();

    Global::s_strTotalScan = QString("%1").arg(m_lstFileTotal.count());
}

void ObjSystemScan::addLostFile(FileListItem* pItem)
{
    QString strSuffix = Tool::getFileSuffix(pItem->m_strFileName);

    if( m_mapFile.find(strSuffix) == m_mapFile.end() )
    {
        CommonFiles cf;
        cf.lstFiles.push_back(pItem);
        cf.strSuffix = strSuffix;

        m_mapFile[strSuffix] = cf;
    }
    else
    {
        m_mapFile[strSuffix].lstFiles.push_back(pItem);
    }
    pItem->m_nIndex = m_lstFileLost.size();

    connect( pItem, &FileListItem::sigDownloadFinished, this, &ObjSystemScan::downloadFinished );
    connect( pItem, &FileListItem::sigCurrentDownloadFile, this, &ObjSystemScan::currentDownloadFile );
    m_lstFileLost.push_back(pItem);
}

void ObjSystemScan::CheckSystemFile()
{

    Global::saveIniData( "LastFixErrorCount", "0" );

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
    Global::saveIniData( "LastScanTime", current_date );


    QString strCurFile;
    int nCount = m_lstFileTotal.count();

    int nPercent = 0;
    bool bFirst = true;

    for( int i = 0; i < nCount; i++ )
    {
        strCurFile = m_lstFileTotal.at(i);

        int nTmp = i*100/nCount;
        //if(nPercent != nTmp or bFirst or nTmp==(nCount-1))
        {
            emit(sigCurrentCheckFile( strCurFile, nCount, i ));
            bFirst = false;
            qDebug()<<"check lost file"<<QThread::currentThreadId();
            nPercent = nTmp;
        }


        if( !Tool::isFileOrDirExist(strCurFile) )
        {
            FileListItem* pItem = new FileListItem();
            pItem->m_strFileName = Tool::getFileName(strCurFile);
            pItem->m_strFilePath = strCurFile;
            addLostFile(pItem);

            emit(sigAddFileToTable(pItem));
        }
    }

    Global::saveIniData( "LastScanErrorCount", QString("%1").arg(m_lstFileLost.count()) );

    m_bChecked = true;
}

void ObjSystemScan::deleteFromTable( int nIndex )
{
//    FileListItem* pItem = m_lstFileLost.at(nIndex);
//    m_lstFileLost.removeAt(nIndex);
//    delete pItem;
//    pItem = NULL;
}

FileListItem* ObjSystemScan::GetLostFileFromList( int nIndex )
{
    return NULL;
    //return m_lstFileLost.at(nIndex);
}

void ObjSystemScan::downloadFile( int nIndex )
{
//    qDebug()<<m_lstFileLost.count()<<"   "<<nIndex;
//    m_lstFileLost.at(nIndex)->downloadFile();
    if(m_bDownload)
    {
        return;
    }


    while(true)
    {
        if( nIndex >= m_lstFileDownloadTmp.size() )
        {
            break;
        }

        if( m_lstFileDownloadTmp.at(nIndex)->m_eStatus == E_STATUS_DOWNLOADING || m_lstFileDownloadTmp.at(nIndex)->m_eStatus == E_STATUS_LOCAL )
        {
            nIndex++;
        }
        else
        {
            break;
        }
    }


    m_bDownload = true;


    if( nIndex >= 0 && nIndex < m_lstFileLost.size() )
    {
        m_lstFileDownloadTmp.at(nIndex)->downloadFile();
    }
}

void ObjSystemScan::downloadFinished(FileListItem* pItem, bool bSuccess)
{
    if(!bSuccess)
    {
        //pItem->m_pPbr->setValue(0);
//        if(pItem->m_nIndex == m_lstFileLost.size() - 1)
//        {
//            m_bDownload = false;
//        }

        if( pItem->m_strDownloadError.lastIndexOf("Not Found") != -1 )
        {
            emit sigDownloadFinished(pItem,bSuccess);
            pItem->m_eStatus = E_STATUS_LOCAL;
            m_nFixedCount++;
        }
        else if( pItem->m_strDownloadError.lastIndexOf("Connection timed out") != -1 )
        {
            emit sigDownloadFinished(pItem,bSuccess);
            return;
        }
    }
    else
    {
        bool bCopySuccess = true;

        if( pItem->m_strFilePath.indexOf( "\\system32\\", 0, Qt::CaseInsensitive) != -1 && Global::s_bX64 )
        {
            PWOW64 pDisable = (PWOW64)GetProcAddress( GetModuleHandle(L"kernel32.dll"), "Wow64DisableWow64FsRedirection" );
            PWOW64 pRevert = (PWOW64)GetProcAddress( GetModuleHandle(L"kernel32.dll"), "Wow64RevertWow64FsRedirection" );
            PVOID OldValue = NULL;
            //Wow64DisableWow64FsRedirection(&OldValue);
            pDisable(&OldValue);

            if(!Tool::isFileOrDirExist(pItem->m_strFilePath))
            {
                Tool::createDirectory(pItem->m_strFilePath, true);
            }

            if(!QFile::copy( pItem->m_strFileDownload, pItem->m_strFilePath ))
            {
                bCopySuccess = false;
            }
            else
            {
                pItem->m_eStatus = E_STATUS_LOCAL;
                m_nFixedCount++;
            }

            //Wow64RevertWow64FsRedirection(OldValue);
            pRevert(OldValue);
        }
        else
        {
            if(!Tool::isFileOrDirExist(pItem->m_strFilePath))
            {
                Tool::createDirectory(pItem->m_strFilePath, true);
            }

            if(!QFile::copy( pItem->m_strFileDownload, pItem->m_strFilePath ))
            {
                bCopySuccess = false;
            }
            else
            {
                pItem->m_eStatus = E_STATUS_LOCAL;
                m_nFixedCount++;
            }
        }

        emit sigDownloadFinished(pItem,bCopySuccess);
    }



    //更新修复文件数
    Global::saveIniData( "LastFixErrorCount", QString("%1").arg(m_nFixedCount) );


    if(Global::s_bActive)
    {
        int nIndex = pItem->m_nIndex + 1;
        if( nIndex >= 0 && nIndex < m_lstFileLost.size() )
        {
            m_bDownload = false;
            downloadFile(m_nCurIndex++);

            return;
        }
    }
    else
    {
        Global::saveFreeFixUnRegister("1");
    }

    m_bDownload = false;
}

void ObjSystemScan::clearLostFile()
{
    for( int i = 0; i < m_lstFileLost.count(); i++ )
    {
        FileListItem* pItem = m_lstFileLost.at(i);
        delete pItem;
        pItem = NULL;
    }

    m_lstFileLost.clear();
}

void ObjSystemScan::currentDownloadFile( FileListItem* pItem, int nPercentage )
{
    //qDebug()<<"ObjSystemScan::CurrentDownloadFile:"<<QThread::currentThreadId();
    emit sigCurrentDownloadFile( pItem, nPercentage );
}

bool ObjSystemScan::isFileNotCheck( QString strFile )
{
    QString strPath = QString("%1\\dllescort\\file.dat").arg(Global::s_strProgramDataPath);

    QFile file(strPath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"open file "<<strPath<<" failed,error:"<<file.errorString();
    }
    else
    {

    }

    QTextStream txtInput(&file);

    bool bNotCheck = false;
    while(!txtInput.atEnd())
    {
        QString strLine = txtInput.readLine();
        if( strLine == strFile )
        {
            bNotCheck = true;
            break;
        }
    }
    file.close();

    return bNotCheck;
}
