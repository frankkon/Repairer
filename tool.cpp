#include "tool.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QTime>
#include <QCoreApplication>
#include <windows.h>

Tool::Tool()
{
}

void Tool::deleteFile( QString strFile )
{
    QFile::remove(strFile);
}



QString Tool::getFileName( QString strFullPath )
{
    strFullPath = strFullPath.replace("/", "\\");
    int nIndex = strFullPath.lastIndexOf( '\\', -1 );

    if( -1 == nIndex )
    {
        return "";
    }

    //QString strTmp = strFullName.right( strFullName.count() - nIndex - 1 );
    return strFullPath.right( strFullPath.count() - nIndex - 1 );
}

QString Tool::getFileNameWithoutSuffix( QString strFile )
{
    strFile = strFile.replace("/", "\\");

    int nIndex1 = strFile.lastIndexOf( '\\', -1 );
    int nIndex2 = strFile.lastIndexOf( '.', -1 );

    if( -1 == nIndex1 )
    {
        nIndex1 = 0;
    }
    if( -1 == nIndex2 )
    {
        nIndex2 = strFile.size();
    }

    return strFile.mid(nIndex1,nIndex2-nIndex1);
}

QString Tool::getFileSuffix( QString strName )
{
    int nIndex = strName.lastIndexOf( '.', -1 );
    return strName.right( strName.count() - nIndex - 1 );
}

void Tool::openAndSelectFile( QString strFile )
{
    QString strCmd = QString("/select,%1").arg(strFile);   // = " /select,D:\\cn_win_srv_2003_r2_enterprise_with_sp2_vl_cd2.iso";
//    QMessageBox::about( NULL, "A", strCmd );
//    QByteArray ba = strCmd.toLatin1();
//    const char *cstr = ba.data();
//    ::ShellExecuteA( 0, "open", "explorer.exe", cstr, NULL, true );

    wchar_t wStr[1024];
    int nLen = strCmd.toWCharArray(wStr);
    wStr[nLen] = 0;

    ::ShellExecuteW( 0, L"open", L"explorer.exe", wStr, NULL, true );
}

void Tool::createSubDirectory( QString strPath )
{
    QDir dir;
    bool bExist = dir.exists(strPath);
    if(!bExist)
    {
        bool bOK = dir.mkdir(strPath);
        if(!bOK)
        {
            QString strMsg = QString("create directory %1 failed").arg(strPath);
            QMessageBox::warning( NULL, "Repairer", strMsg );
        }
    }
}

void Tool::createDirectory( QString strPath, bool bFile )
{
    QString strDir;

    int nIndex = -1;
    int nFrom = 0;
    do
    {
        bool bBreak = false;
        nIndex = strPath.indexOf( '\\', nFrom );
        if( -1 == nIndex )
        {
            if(bFile)
            {
                break;
            }
            else
            {
                bBreak = true;
            }
        }

        if(bBreak)
        {
            createSubDirectory(strPath);
            break;
        }
        else
        {
            nFrom = nIndex + 1;
            strDir = strPath.left( nIndex + 1 );
            createSubDirectory(strDir);
        }

    }while(true);
}


void Tool::uncompressFile( QString strCompressedFile, QString strSavePath, QString str7zPath )
{
    qDebug()<<"uncompressFile begin";
    QString strCmd = QString("x \"%1\" -o\"%2\" -y").arg(strCompressedFile).arg(strSavePath);  //-o后面不能有空格

//    QByteArray ba = strCmd.toLatin1();
//    const char *cstr = ba.data();
//    ::ShellExecuteA( 0, "open", "7z.exe", cstr, NULL, SW_HIDE );

    wchar_t wStr[1024];
    int nLen = strCmd.toWCharArray(wStr);
    wStr[nLen] = 0;

    wchar_t wStr7z[1024];
    int nLen7z = str7zPath.toWCharArray(wStr7z);
    wStr7z[nLen7z] = 0;

    ::ShellExecute( 0, L"open", wStr7z, wStr, NULL, SW_HIDE );

    //等待解压缩完成
    Sleep(5000);

    qDebug()<<"uncompressFile end";
}

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

QString Tool::getSystemInfo()
{
    QString strBit;
    SYSTEM_INFO info;
    ::GetSystemInfo(&info);
    if( info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 )
    {
        strBit = "64-bit";
    }
    else if( info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL )
    {
        strBit = "32-bit";
    }
    else
    {
        strBit.clear();
    }

    QString strOs;
    OSVERSIONINFOEX osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx((OSVERSIONINFO*)&osInfo);

    PGPI pGPI;
    DWORD dwType;

    switch(osInfo.dwMajorVersion)
    {
    case 5:
        {
            switch(osInfo.dwMinorVersion)
            {
            case 0:
                {
                    //Windows 2000
                    strOs = "Windows 2000";
                }
                break;
            case 1:
                {
                    //Windows XP
                    strOs = "Windows XP";
                    if( osInfo.wSuiteMask == VER_SUITE_EMBEDDEDNT )   //Embedded 按照 Home Edition
                    {
                        strOs += "Home Edition";
                    }
                    else if( osInfo.wSuiteMask == VER_SUITE_PERSONAL )
                    {
                        strOs += "Home Edition";
                    }
                    else
                    {
                        strOs += "Professional";
                    }
                }
                break;
            case 2:
                {
                    if( VER_NT_WORKSTATION == osInfo.wProductType && PROCESSOR_ARCHITECTURE_AMD64 == info.wProcessorArchitecture )
                    {
                        strOs = "Windows XP Professional x64 Edition";
                    }
                    else
                    {

                    }
                }
                break;
            default:
                {
                    strOs = "other os";
                }
                break;
            }
        }
        break;
    case 6:
        {
            switch(osInfo.dwMinorVersion)
            {
            case 0:
                {
                    if( VER_NT_WORKSTATION == osInfo.wProductType )
                    {
                        //Windows Vista
                        strOs = "Windows Vista";
                    }
                    else
                    {
                        //Windows Server 2008
                        strOs = "Windows Server 2008";
                    }
                }
                break;
            case 1:
                {
                    if( VER_NT_WORKSTATION == osInfo.wProductType )
                    {
                        //Windows 7
                        strOs = "Windows 7";
                    }
                    else
                    {
                        //Windows Server 2008 R2
                        strOs = "Windows Server 2008 R2";
                    }
                }
            break;
            case 2:
                {
                    //Windows 8
                    strOs = "Windows 8";

                    pGPI = (PGPI)GetProcAddress( GetModuleHandle(L"kernel32.dll"), "GetProductInfo" );
                    pGPI( osInfo.dwMajorVersion, osInfo.dwMinorVersion, 0, 0, &dwType );

                    if( PRODUCT_PROFESSIONAL == dwType )
                    {
                        strOs += " Pro";
                    }
                    else if( PRODUCT_ENTERPRISE == dwType )
                    {
                        strOs += " Enterprise";
                    }
                }
                break;
            default:
                {
                }
            }
        }
        break;
    case 7:
        {

        }
        break;
    default:
        break;
    }


    qDebug()<<"numbers of CPUs:"<<info.dwNumberOfProcessors;
    qDebug()<<"CPU type:"<<info.dwProcessorType<<info.wProcessorRevision;

    QString str;
    return str;
}

QString Tool::getFileVersion( QString strFile )
{
    //open开始
    QByteArray ba = strFile.toLatin1();
    const char *cstr = ba.data();

    //Get the version information size for allocate the buffer
    DWORD dwHandle;
    DWORD dwDataSize = ::GetFileVersionInfoSizeA((LPCSTR)cstr, &dwHandle);
    //DWORD dwDataSize = ::GetFileVersionInfoSizeA( "C:\\a", &dwHandle );
    if( 0 == dwDataSize )
    {
        DWORD dwError = ::GetLastError();
        qDebug()<<dwError;
        return "";
    }

    //Allocate buffer and retrieve version information
    BYTE* pVersionData = new BYTE[dwDataSize];
    if (!::GetFileVersionInfoA((LPCSTR)cstr, dwHandle, dwDataSize, (void**)pVersionData) )
    {
        delete[] pVersionData;
        pVersionData = NULL;
        return "";
    }

    //Retrieve the first language and character-set identifier
    UINT nQuerySize;
    DWORD* pTransTable;
    if (!::VerQueryValue(pVersionData, L"\\VarFileInfo\\Translation", (void **)&pTransTable, &nQuerySize) )
    {
        delete[] pVersionData;
        pVersionData = NULL;
        return "";
    }
    //open结束

    nQuerySize = 0;
    VS_FIXEDFILEINFO* pVsffi;
    if ( ::VerQueryValueA((void **)pVersionData, "\\", (void**)&pVsffi, &nQuerySize) )
    {

    }


    QString strVersion = QString("%1.%2.%3.%4").arg(HIWORD(pVsffi->dwFileVersionMS)).arg(LOWORD(pVsffi->dwFileVersionMS)).arg(HIWORD(pVsffi->dwFileVersionLS)).arg(LOWORD(pVsffi->dwFileVersionLS));

    return strVersion;




    //Swap the words to have lang-charset in the correct format
//    DWORD dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));
//    //QString strV = Tool::queryValue( "FileVersion", dwLangCharset, pVersionData );  //ProductVersion
//    QString strV = Tool::queryValue( "ProductVersion", dwLangCharset, pVersionData );  //ProductVersion

//    //此时可能出现 v_6.1.7600.16385 (win7_rtm.090713-1255)
//    int nIndex = strV.indexOf(' ');
//    if( nIndex != -1 )
//    {
//        strV = strV.left(nIndex);
//    }

//    return strV;
}

QString Tool::queryValue( QString strValueName, DWORD dwLangCharset, BYTE* pVersionData )
{
    if ( pVersionData == NULL )
        return "";

    //Query version information value
    UINT nQuerySize;
    LPVOID lpData;

    QString strTmp;
    //QString strTmp = QString("%08lx");
    strTmp.sprintf( "%08lx", dwLangCharset );
    QString strBlockName = QString("\\StringFileInfo\\%1\\%2").arg(strTmp).arg(strValueName);
    QByteArray ba = strBlockName.toLatin1();
    const char *cstr = ba.data();

    if ( ::VerQueryValueA((void **)pVersionData, cstr, &lpData, &nQuerySize) )
    {

    }

    QString strValue = QString(QLatin1String((char*)lpData));

    return strValue;
}

bool Tool::isFileTypeX86( QString strFile )
{
    wchar_t wStr[1024];
    int nLen = strFile.toWCharArray(wStr);
    wStr[nLen] = 0;

    IMAGE_DOS_HEADER idh;
//    FILE *f = fopen(lpFileName, "rb");
    FILE* f = _wfopen(wStr, TEXT("rb"));
//    FILE *f = NULL;
//    _wfopen_s(&f, wStr, TEXT("rb"));
    fread(&idh, sizeof(idh), 1, f);

    IMAGE_FILE_HEADER ifh;
    fseek(f, idh.e_lfanew + 4, SEEK_SET);
    fread(&ifh, sizeof(ifh), 1, f);
    fclose(f);


    if(ifh.Machine == 0x014C)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Tool::getFileSize( QString strFile )
{
    QFile file(strFile);
    if (!file.open(QIODevice::ReadOnly))
        return 0;

    int nSize = file.size()/1024;
    file.close();

    return nSize;
}

qint64 Tool::getFileSizeByte( QString strFile )
{
    QFile file(strFile);
    if (!file.open(QIODevice::ReadOnly))
        return 0;

    qint64 nSize = file.size();
    file.close();

    return nSize;
}

void Tool::removeFilesinDir(const QString path)
{
  QDir dir(path);
  QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs);
  foreach(QFileInfo file_info, info_list)
  {
    if (file_info.isDir())
    {
      Tool::removeFilesinDir(file_info.absoluteFilePath());
    }
    else if (file_info.isFile())
    {
        QFile file(file_info.absoluteFilePath());
        file.setPermissions(QFile::WriteOwner);
        qDebug() << "remove file  : " << file_info.absoluteFilePath();
        file.remove();
    }
  }
  QDir temp_dir;
  temp_dir.rmdir(path) ;
  qDebug() << "remove empty dir : " << path;
}

void Tool::reomveDir( QString strPath )
{
//    QDir dir(strPath);
//    dir.setFilter(QDir::Files);
//    for( int i=0; i <= dir.count()-1; i++ )
//        dir.remove(dir[i]);

    if(!isFileOrDirExist(strPath))
    {
        return;
    }

    QDir dir;
    QFileInfoList list;
    QFileInfoList::iterator it;
    QVector<QString> dirNames;

    dirNames<<strPath;

    for( int i=0; i<dirNames.size(); ++i )
    {
        dir.setPath(dirNames[i]);
        list = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden|QDir::NoDotAndDotDot, QDir::Name);
        if(list.size()>0)
        {
            it = list.begin();
            while(it!=list.end())
            {
                if(it->isDir())
                {
                    dirNames.push_back(it->filePath());
                }
                else
                {
                    QFile file(it->fileName());
                    file.setPermissions(QFile::WriteOwner);
                    file.close();

                    if(!dir.remove(it->fileName()))
                    {
                        QString str = QString("remove file %1 failed!").arg(it->fileName());
                        qDebug()<<str;
                    }
                }

                it++;
            }
        }
    }

    for( int i=dirNames.size()-1; i>=0; --i )
    {
        dir.setPath(dirNames[i]);
        if(!dir.rmdir("."))
        {
            QString str = QString("remove dir %1 failed!").arg(dirNames[i]);
            //QMessageBox::about( NULL, "warning", str );

            qDebug()<<str;
        }
    }
}

#include "global.h"
bool Tool::isFileOrDirExist( QString strFile )
{
    if( strFile.indexOf( "\\system32\\", 0, Qt::CaseInsensitive) != -1 && Global::s_bX64 )
    {
        PWOW64 pDisable = (PWOW64)GetProcAddress( GetModuleHandle(L"kernel32.dll"), "Wow64DisableWow64FsRedirection" );
        PWOW64 pRevert = (PWOW64)GetProcAddress( GetModuleHandle(L"kernel32.dll"), "Wow64RevertWow64FsRedirection" );
        PVOID OldValue = NULL;
        //Wow64DisableWow64FsRedirection(&OldValue);
        pDisable(&OldValue);

        bool bRtn = false;
        QFileInfo fileInfo(strFile);
        bRtn = fileInfo.exists();

        //Wow64RevertWow64FsRedirection(OldValue);
        pRevert(OldValue);

        return bRtn;
    }
    else
    {
        QFileInfo fileInfo(strFile);
        return fileInfo.exists();
    }
}
