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

    m_iTotalScanedFiles = 0;
    m_iTotalScanedRegs = 0;
    scanAllTrashFiles();
    scanAllRegErrors();

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

    //获取需要扫描的路径信息
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
    qDebug() << "扫描所有注册表错误:scanAllRegErrors()";

    //获取需要扫描的注册表路径
    QList<TRegScanInfo*>* pList = m_pDiskScanConfig->getRegScanInfo();
    if(NULL != pList)
    {
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
            QSettings reg(pScanInfo->reg_path);
            QStringList keyList = pScanInfo->err_list;
            foreach(QString key, keyList)
            {
                reg.remove(key);
                emit sigNotifyUIUpdateCleanProgress(keyList.count(),
                                                    pScanInfo->desc_en,
                                                    pScanInfo->reg_path + key);
            }
        }
    }
}

//扫描指定的磁盘目录
void ObjDiskClean::scanTrashFilesInPath(TDiskScanInfo* scanInfo)
{
    qDebug() << "清理指定目录的磁盘垃圾:scanTrashFilesInPath()";
    qDebug() << "正在扫描" << scanInfo->desc_cn;
    qDebug() << scanInfo->trash_path;

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

//替换路径中的环境遍历
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

    DWORD dw = 0;
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
        ::RegGetValue(hkResult, NULL, (PCWSTR)key.constData(),
                      RRF_RT_DWORD, NULL, &dw, &sdw);

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

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    foreach(QString key, keyList)
    {
        //没有路径标识的键值不确定，暂不处理
        if(!key.contains("/"))
        {
            continue;
        }

        QFile file(key);

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

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    foreach(QString key, keyList)
    {
        QString value = reg.value(key).toString();
        value = value.replace("\"", "");
        int idx = value.indexOf(".exe", 0, Qt::CaseInsensitive);
        if(idx < 0) //没有.exe的情况比较复杂，不处理
        {
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
    //通过MsiExec.exe或者RunDll32卸载的不处理
    //其他情况的不处理。

    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);
    QStringList groupList = reg.childGroups();
    int groupCount = groupList.count();
    foreach(QString group, groupList)
    {
        reg.beginGroup(group);
        QStringList lstchildKeys = reg.childKeys();
        if(lstchildKeys.empty())
        {
            scanInfo->err_list.append(group);
            m_iTotalScanedRegs++;
            reg.endGroup();
            continue;
        }

        if(lstchildKeys.contains("UninstallString"))
        {
            QString value = reg.value("UninstallString").toString();
            if(value.contains("MsiExec.exe", Qt::CaseInsensitive)
                    || value.contains("RunDll32", Qt::CaseInsensitive))
            {
                reg.endGroup();
                continue;
            }

            value.replace("\"", ""); //有些路径包含双引号，先去掉
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

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    foreach(QString key, keyList)
    {
        QString value = reg.value(key).toString();
        value = value + "/" + key;
        replaceEnv(value);  //有些值的目录里面有环境变量。

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

    QStringList keyList = reg.childKeys();
    int keyCount = keyList.count();
    foreach(QString key, keyList)
    {
        QString path = key;
        replaceEnv(path);  //有些值的目录里面有环境变量。

        QDir dir(path);
        if(!dir.exists())  //如果文件不存在，则需要删除
        {
            scanInfo->err_list.append(key);
            m_iTotalScanedRegs++;
        }
        emit sigNotifyUIUpdateScanProgress(keyCount, scanInfo->desc_en, key);
    }
}

void ObjDiskClean::scanRegByAppPath(TRegScanInfo* /*scanInfo*/)
{
    //检查逻辑不确定，没有统一的规律，检查默认键的值不对。
}

void ObjDiskClean::scanRegByResidualInstall(TRegScanInfo* scanInfo)
{
    //Registry64Format保证兼容32位和64位系统
    QSettings reg(scanInfo->reg_path, QSettings::Registry64Format);

    QStringList groupList = reg.childGroups();
    int groupCount = groupList.count();
    foreach(QString group, groupList)
    {
        //Policies这个目录比较特别，防止有问题，忽略
        //这里先硬编码，如果类似目录比较多，后续做成白名单。
        if(group.contains("Policies", Qt::CaseInsensitive))
        {
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





