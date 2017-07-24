#include "objdiskclean.h"
#include "tool.h"
#include "global.h"
#include "Log.h"

#include <QDebug>
#include <QDir>
#include <QThread>
#include <QSettings>
#include <QList>
#include <QProcessEnvironment>
#include <QRegExp>

//以盘符打头的路径
#define FULL_PATH_A "^[a-zA-Z]:\\\\"

//以环境变量打头的路径
#define FULL_PATH_B "^%[a-zA-Z]*%\\\\"

ObjDiskClean::ObjDiskClean(QObject* parent) :
    QObject(parent),
    m_iTotalScanedFiles(0),
    m_dTotalScanedFileSize(0),
    m_iTotalScanedRegs(0)
{
    m_pDiskScanConfig = CDiskScanConfig::loadConfig();    
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
    CLog::getInstance()->logDebug("......ObjDiskClean::scanAllErrors()......");

    m_iTotalScanedFiles = 0;
    m_iTotalScanedRegs = 0;
    scanAllTrashFiles();
    scanAllRegErrors();

#ifndef QT_NO_DEBUG
    dumpScanFile();
    dumpScanReg();
#endif

    //完成所有扫描后通知UI
    emit sigNotifyUIScanFinished(m_iTotalScanedFiles,
                                 m_dTotalScanedFileSize, m_iTotalScanedRegs);
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
    CLog::getInstance()->logDebug("......ObjDiskClean::scanAllTrashFiles()......");

    //获取需要扫描的路径信息
    QList<TDiskScanInfo*>* pList = m_pDiskScanConfig->getDiskScanInfo();
    if(NULL != pList)
    {
        CLog::getInstance()->logDebug("......开始逐个路径扫描垃圾文件......");
        QListIterator<TDiskScanInfo*> it(*pList);
        while(it.hasNext())
        {
            scanTrashFilesInPath(it.next());
        }
    }
}

void ObjDiskClean::scanAllRegErrors()
{
    qDebug() << "扫描所有注册表错误:scanAllRegErrors()";
    CLog::getInstance()->logDebug("......ObjDiskClean::scanAllRegErrors()......");

    //获取需要扫描的注册表路径
    QList<TRegScanInfo*>* pList = m_pDiskScanConfig->getRegScanInfo();
    if(NULL != pList)
    {
        CLog::getInstance()->logDebug("......开始逐个路径扫描注册表......");
        QListIterator<TRegScanInfo*> it(*pList);
        while(it.hasNext())
        {
            scanRegInPath(it.next());
        }
    }
}

void ObjDiskClean::cleanAllTrashFiles()
{
    QList<TDiskScanInfo*>* pList = m_pDiskScanConfig->getDiskScanInfo();
    if(NULL != pList)
    {
        QListIterator<TDiskScanInfo*> it(*pList);
        while(it.hasNext())
        {
            TDiskScanInfo* pScanInfo = it.next();
            QStringList fileList = pScanInfo->file_list;
            foreach(QString file, fileList)
            {
                QFile sPath(file);
                if(sPath.exists())
                {
                    sPath.remove();
                }

                emit sigNotifyUIUpdateCleanProgress(fileList.count(),
                                                    pScanInfo->desc_en,
                                                    file);
            }
            pScanInfo->file_list.clear();
        }
    }

}

void ObjDiskClean::cleanAllRegErrors()
{
    QList<TRegScanInfo*>* pList = m_pDiskScanConfig->getRegScanInfo();
    if(NULL != pList)
    {
        QListIterator<TRegScanInfo*> it(*pList);
        while(it.hasNext())
        {
            TRegScanInfo* pScanInfo = it.next();
            QSettings reg(pScanInfo->reg_path, QSettings::Registry64Format);
            QStringList keyList = pScanInfo->err_list;
            foreach(QString key, keyList)
            {
                reg.remove(key);
                emit sigNotifyUIUpdateCleanProgress(keyList.count(),
                                                    pScanInfo->desc_en,
                                                    pScanInfo->reg_path + key);
            }
            pScanInfo->err_list.clear();
        }
    }
}

//扫描指定的磁盘目录
void ObjDiskClean::scanTrashFilesInPath(TDiskScanInfo* scanInfo)
{
    qDebug() << "清理指定目录的磁盘垃圾:scanTrashFilesInPath()";
    qDebug() << "正在扫描" << scanInfo->desc_cn;
    qDebug() << scanInfo->trash_path;
    CLog::getInstance()->logDebug("......ObjDiskClean::scanTrashFilesInPath......");
    CLog::getInstance()->logDebug(scanInfo->trash_path.toStdString().c_str());

    scanInfo->total_size = 0;
    scanInfo->file_count = 0;
    scanInfo->file_list.clear();

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
        scanDir.setFilter(QDir::Dirs|QDir::Files|QDir::NoSymLinks
                          |QDir::NoDotAndDotDot|QDir::Hidden|QDir::System);

        //不能判空
        //if(scanDir.isEmpty())
        //{
        //    continue;
        //}

        //目录时按照文件通配符筛选垃圾文件
        if(0 == scanInfo->clean_all_flag)
        {
            extNameList = scanInfo->ext_name.split(";");
            scanDir.setNameFilters(extNameList);
        }

        //遍历当前目录
        scanDirFiles = scanDir.entryInfoList();
        QListIterator<QFileInfo> itFile(scanDirFiles);
        while(itFile.hasNext())
        {
            fileInfo = itFile.next();
            if(fileInfo.isFile())
            {
                scanInfo->total_size += (double)fileInfo.size()/1024/1024; //转换成MB
                scanInfo->file_count++;
                m_iTotalScanedFiles++;
                scanInfo->file_list.append(fileInfo.filePath());
                emit sigNotifyUIUpdateScanProgress(scanDir.count(),
                                                   scanInfo->desc_en,
                                                   fileInfo.filePath());
            }
            else
            {
                //如果是目录，就继续加到待扫描的目录列表中
                scanDirList.append(fileInfo.filePath());
            }
        }
    }

    //扫描完成后，总计垃圾文件大小
    m_dTotalScanedFileSize += scanInfo->total_size;

}

//替换路径中的环境变量
void ObjDiskClean::replaceEnv(QString& srcDir)
{
    //只能处理只有一个环境变量的情况

    if(srcDir.contains("%"))
    {
        int begin = srcDir.indexOf("%");
        int end = srcDir.lastIndexOf("%");
        QString env = srcDir.mid(begin + 1, end - begin - 1);
        QString path = QProcessEnvironment::systemEnvironment().value(env);
        srcDir.replace(begin, (end - begin + 1), path);
    }
}

//扫描指定的注册表路径
void ObjDiskClean::scanRegInPath(TRegScanInfo* scanInfo)
{
    qDebug() << "扫描指定的注册表路径:scanRegInPath()";
    qDebug() << "正在扫描" << scanInfo->desc_cn;
    qDebug() << scanInfo->reg_path;
    CLog::getInstance()->logDebug("......ObjDiskClean::scanRegInPath......");
    CLog::getInstance()->logDebug(scanInfo->reg_path.toStdString().c_str());

    scanInfo->err_count = 0;
    scanInfo->err_list.clear();

    switch (scanInfo->err_type)
    {
        case DLL_ERROR:
        {
            scanRegByDllErrors(scanInfo);
            break;
        }
        case APP_COMPAT:
        {
            scanRegByAppCompat(scanInfo);
            break;
        }
        case INVALID_STARTUP:
        {
            scanRegByInvalidStartup(scanInfo);
            break;
        }
        case RESIDUAL_UNINSTALL:
        {
            scanRegByResidualUninstall(scanInfo);
            break;
        }
        case WRONG_HELP:
        {
            scanRegByWrongHelp(scanInfo);
            break;
        }
        case APP_INSTALL:
        {
            scanRegByAppInstall(scanInfo);
            break;
        }
        case APP_PATH:
        {
            scanRegByAppPath(scanInfo);
            break;
        }
        case RESIDUAL_INSTALL:
        {
            scanRegByResidualInstall(scanInfo);
            break;
        }
        default:
        {
            break;
        }
    }
}

void ObjDiskClean::scanRegByDllErrors(TRegScanInfo* scanInfo)
{
    //这里要用windows API 读取注册表，QSettings不支持读取带有反斜杠(\)的键的值。

    QString regPath = scanInfo->reg_path;

    //Registry64Format保证兼容32位和64位系统
    QSettings reg(regPath, QSettings::Registry64Format);
    //QSettings reg(regPath, QSettings::NativeFormat);

    DWORD dw = 0;
    DWORD dwType = 0;
    DWORD sdw = sizeof(dw);
    HKEY hkResult = NULL;
    LONG rt = 0;
    int idx = 0;
    QString sLeft;
    QString sRight;
    idx = regPath.indexOf('\\');
    sLeft = regPath.left(idx);
    sRight = regPath.mid(idx + 1);
    HKEY hkPrimary = getPrimaryKeyHandle(sLeft);
    rt = ::RegOpenKeyEx(hkPrimary, (LPCWSTR)sRight.constData(), 0,
                        KEY_ALL_ACCESS|KEY_WOW64_64KEY, &hkResult);
    if(ERROR_SUCCESS != rt)
    {
        return;
    }

    QString sOrginKey;
    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    foreach(QString key, keyList)
    {
        //在替换反斜杠之前先保存下来
        sOrginKey = key;
        key = key.replace("/","\\");

        //保持对Windows XP x86的兼容，在Windows XP x86下没有次函数
        //::RegGetValue(hkResult, NULL, (PCWSTR)key.constData(),
        //              RRF_RT_DWORD, NULL, &dw, &sdw);
        ::RegQueryValueEx(hkResult, (PCWSTR)key.constData(), NULL, &dwType, (LPBYTE)(&dw), &sdw);
        if(REG_DWORD != dwType)
        {
            emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
            continue;
        }

        //如果引用计数为0，则需要删除
        if(0 == dw)
        {
            scanInfo->err_list.append(sOrginKey);
            m_iTotalScanedRegs++;
        }
        emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
    }

    ::RegCloseKey(hkResult);
}

void ObjDiskClean::scanRegByAppCompat(TRegScanInfo* scanInfo)
{
    //Registry64Format保证兼容32位和64位系统
    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    QRegExp expA(FULL_PATH_A);
    QRegExp expB(FULL_PATH_B);

    foreach(QString key, keyList)
    {
        QString tmpKey = key;

        tmpKey.replace("\"", ""); //有些路径包含双引号，先去掉
        replaceEnv(tmpKey);  //有些值的目录里面有环境变量。

        //不是完整路径的键值不确定能否删除，暂不处理
        if(!tmpKey.contains(expA) && !tmpKey.contains(expB))
        {
            emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
            continue;
        }

        QFile file(tmpKey);

        //如果文件不存在，则需要删除
        if(!file.exists())
        {
            scanInfo->err_list.append(key);
            m_iTotalScanedRegs++;
        }
        emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
    }
}

void ObjDiskClean::scanRegByInvalidStartup(TRegScanInfo* scanInfo)
{
    //Registry64Format保证兼容32位和64位系统
    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    QRegExp expA(FULL_PATH_A);
    QRegExp expB(FULL_PATH_B);

    foreach(QString key, keyList)
    {
        QString value = reg.value(key).toString();
        value = value.replace("\"", "");
        int idx = value.indexOf(".exe", 0, Qt::CaseInsensitive);

        //没有.exe的情况比较复杂，不处理
        //不是完整路径的不处理
        if(idx < 0 || (!value.contains(expA) && !value.contains(expB)))
        {
            emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
            continue;
        }

        QString sLeft = value.left(idx + 4);
        replaceEnv(sLeft);  //有些值的目录里面有环境变量。

        QFile file(sLeft);
        if(!file.exists())  //如果文件不存在，则需要删除
        {
            scanInfo->err_list.append(key);
            m_iTotalScanedRegs++;
        }
        emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
    }
}

void ObjDiskClean::scanRegByResidualUninstall(TRegScanInfo* scanInfo)
{
    //group里面没有键，将group加入待删除列表
    //UninstallString对应的执行文件不存在的，将group加入待删除列表
    //不是完整路径的不处理
    //其他情况的不处理。

    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);
    QStringList groupList = reg.childGroups();
    int groupCount = groupList.count();
    QRegExp expA(FULL_PATH_A);
    QRegExp expB(FULL_PATH_B);

    foreach(QString group, groupList)
    {
        reg.beginGroup(group);
        QStringList lstchildKeys = reg.childKeys();
        if(lstchildKeys.empty())
        {
            scanInfo->err_list.append(group);
            m_iTotalScanedRegs++;
            reg.endGroup();
            emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
            continue;
        }

        if(lstchildKeys.contains("UninstallString", Qt::CaseInsensitive ))
        {
            QString value = reg.value("UninstallString").toString();
            value.replace("\"", ""); //有些路径包含双引号，先去掉

            if(!value.contains(expA) && !value.contains(expB))
            {
                reg.endGroup();
                emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
                continue;
            }

            int idx = value.indexOf(".exe", 0, Qt::CaseInsensitive);
            if(-1 != idx)
            {
                QString sLeft = value.left(idx + 4);
                replaceEnv(sLeft);  //有些值的目录里面有环境变量。

                QFile file(sLeft);
                if(!file.exists())  //如果文件不存在，则需要删除
                {
                    scanInfo->err_list.append(group);
                    m_iTotalScanedRegs++;
                }
            }
        }

        reg.endGroup();
        emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
    }
}

void ObjDiskClean::scanRegByWrongHelp(TRegScanInfo* scanInfo)
{
    //Registry64Format保证兼容32位和64位系统
    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    QRegExp expA(FULL_PATH_A);
    QRegExp expB(FULL_PATH_B);

    foreach(QString key, keyList)
    {
        QString value = reg.value(key).toString();
        value.replace("\"", ""); //有些路径包含双引号，先去掉
        replaceEnv(value);  //有些值的目录里面有环境变量。
        value = value + "/" + key; //正斜杠反斜杠都一样，qt会自动转换

        //不是完整路径的不处理
        if(!value.contains(expA) && !value.contains(expB))
        {
            emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
            continue;
        }

        QFile file(value);
        if(!file.exists())  //如果文件不存在，则需要删除
        {
            scanInfo->err_list.append(key);
            m_iTotalScanedRegs++;
        }
        emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
    }
}

void ObjDiskClean::scanRegByAppInstall(TRegScanInfo* scanInfo)
{
    //Registry64Format保证兼容32位和64位系统
    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    QRegExp expA(FULL_PATH_A);
    QRegExp expB(FULL_PATH_B);

    foreach(QString key, keyList)
    {
        QString tmpKey = key;
        tmpKey.replace("\"", ""); //有些路径包含双引号，先去掉
        replaceEnv(tmpKey);  //有些值的目录里面有环境变量。

        //不是完整路径的不处理
        if(!tmpKey.contains(expA) && !tmpKey.contains(expB))
        {
            emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
            continue;
        }

        QDir dir(tmpKey);
        if(!dir.exists())  //如果文件夹不存在，则需要删除
        {
            scanInfo->err_list.append(key);
            m_iTotalScanedRegs++;
        }
        emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
    }
}

void ObjDiskClean::scanRegByAppPath(TRegScanInfo* scanInfo)
{
    //读取注册项下的“默认”键的值
    //如果“默认”键的值位完整路径，则检查是否有效，否则忽略
    //如果完整路径指示的文件不存在，则将该注册项加入待删除列表

    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);

    QStringList groupList = reg.childGroups();
    int groupCount = groupList.count();
    QRegExp expA(FULL_PATH_A);
    QRegExp expB(FULL_PATH_B);
    QRegExp endOfExe("\\.[eE][xX][eE]$");

    foreach(QString group, groupList)
    {
        reg.beginGroup(group);
        QString defaultKeyValue = reg.value(".").toString(); //读取注册项下的“默认”键值
        defaultKeyValue.replace("\"", ""); //有些路径包含双引号，先去掉
        replaceEnv(defaultKeyValue);  //有些值的目录里面有环境变量。

        //不是完整路径的不处理,结尾不是exe执行文件的不处理
        if( (!defaultKeyValue.contains(expA) && !defaultKeyValue.contains(expB))
                || !defaultKeyValue.contains(endOfExe))
        {
            reg.endGroup();
            emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
            continue;
        }

        QFile file(defaultKeyValue);
        if(!file.exists())  //如果文件不存在，则需要删除
        {
            scanInfo->err_list.append(group);
            m_iTotalScanedRegs++;
        }
        reg.endGroup();
        emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
    }
}

void ObjDiskClean::scanRegByResidualInstall(TRegScanInfo* scanInfo)
{
    //Registry64Format保证兼容32位和64位系统
    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    //QSettings reg(scanInfo->reg_path, QSettings::NativeFormat);

    QStringList groupList = reg.childGroups();
    int groupCount = groupList.count();
    foreach(QString group, groupList)
    {
        //Policies这个目录比较特别，防止有问题，忽略
        //这里先硬编码，如果类似目录比较多，后续做成白名单。
        if(group.contains("Policies", Qt::CaseInsensitive)
                || group.contains("Microsoft", Qt::CaseInsensitive))
        {
            emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
            continue;
        }

        reg.beginGroup(group);

        QStringList lstKeys = reg.allKeys();
        if(lstKeys.empty())
        {
            scanInfo->err_list.append(group);
            m_iTotalScanedRegs++;
        }

        reg.endGroup();
        emit sigNotifyUIUpdateScanProgress(groupCount, scanInfo->desc_en, group);
    }
}

HKEY ObjDiskClean::getPrimaryKeyHandle(QString sKey)
{
    if("HKEY_CURRENT_USER" == sKey)
    {
        return HKEY_CURRENT_USER;
    }
    else if("HKEY_LOCAL_MACHINE" == sKey)
    {
        return HKEY_LOCAL_MACHINE;
    }
    else if("HKEY_USERS" == sKey)
    {
        return HKEY_USERS;
    }
    else if("HKEY_CLASSES_ROOT" == sKey)
    {
        return HKEY_CLASSES_ROOT;
    }
    else if("HKEY_CURRENT_CONFIG" == sKey)
    {
        return HKEY_CURRENT_CONFIG;
    }
    else if("HKEY_DYN_DATA" == sKey)
    {
        return HKEY_DYN_DATA;
    }
    else if("HKEY_PERFORMANCE_DATA" == sKey)
    {
        return HKEY_PERFORMANCE_DATA;
    }
    else if("HKEY_PERFORMANCE_NLSTEXT" == sKey)
    {
        return HKEY_PERFORMANCE_NLSTEXT;
    }
    else if("HKEY_PERFORMANCE_TEXT" == sKey)
    {
        return HKEY_PERFORMANCE_TEXT;
    }
    else
    {
        return 0;
    }
}


#ifndef QT_NO_DEBUG
void ObjDiskClean::dumpScanFile()
{
    QFile file("dumpFile.txt");
    file.open(QFile::WriteOnly | QFile::Truncate);
    QTextStream out(&file);
    QList<TDiskScanInfo*>* pList = m_pDiskScanConfig->getDiskScanInfo();
    if(NULL != pList)
    {
        QListIterator<TDiskScanInfo*> it(*pList);
        while(it.hasNext())
        {
            out << "===================================================" << endl;
            TDiskScanInfo* scaninfo = it.next();
            out << "=========" << scaninfo->desc_cn << "=============" << endl;
            foreach(QString str, scaninfo->file_list)
            {
                out << str << endl;
            }
            out << "===================================================" << endl;
        }
    }}

void ObjDiskClean::dumpScanReg()
{
    QFile file("dumpReg.txt");
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream stream(&file);
        QList<TRegScanInfo*>* pList = m_pDiskScanConfig->getRegScanInfo();
        if(NULL != pList)
        {
            QListIterator<TRegScanInfo*> it(*pList);
            while(it.hasNext())
            {
                stream << "============================" << endl;
                TRegScanInfo* p = it.next();
                QStringList lst = p->err_list;
                stream << "====" << p->desc_cn << "====" << endl;
                foreach(QString str, lst)
                {
                    stream << str << endl;
                }
                stream << "============================" << endl;

            }
        }
    }
}
#endif



