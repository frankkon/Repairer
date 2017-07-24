#include "windows.h"

#include "global.h"
#include "cdiskscanconfig.h"
#include "cwin10x86diskscanconfig.h"
#include "cwin10x64diskscanconfig.h"
#include "cwinxpx64diskscanconfig.h"
#include "cwinxpx86diskscanconfig.h"
#include "cwinvistax64diskscanconfig.h"
#include "cwinvistax86diskscanconfig.h"
#include "cwin7x64diskscanconfig.h"
#include "cwin7x86diskscanconfig.h"
#include "cwin81x64diskscanconfig.h"
#include "cwin81x86diskscanconfig.h"
#include "cwin8x64diskscanconfig.h"
#include "cwin8x86diskscanconfig.h"
#include "Log.h"

#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QString>
#include <QDebug>

CDiskScanConfig::CDiskScanConfig()
{
    m_dbConnect = QSqlDatabase::addDatabase("QSQLITE");
    m_dbConnect.setDatabaseName(CONFIG_DB_NAME);
}

CDiskScanConfig::~CDiskScanConfig()
{
    if(!m_lstDiskScanInfo.isEmpty())
    {
        QListIterator<TDiskScanInfo*> it(m_lstDiskScanInfo);
        while (it.hasNext())
        {
           delete it.next();
        }
    }

    if(!m_lstRegScanInfo.isEmpty())
    {
        QListIterator<TRegScanInfo*> it(m_lstRegScanInfo);
        while (it.hasNext())
        {
           delete it.next();
        }
    }
}

CDiskScanConfig* CDiskScanConfig::loadConfig()
{
    int platform = Global::getWindowsVersionInfo();

    switch (platform)
    {
        case WINDOWS_XP_X86:
        {
            CLog::getInstance()->logDebug("......创建CWinXpX86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWinXpX86DiskScanConfig();
        }
        case WINDOWS_XP_X64:
        {
            CLog::getInstance()->logDebug("......创建CWinXpX64DiskScanConfig，初始化垃圾扫描信息......");
            return new CWinXpX64DiskScanConfig();
        }
        case WINDOWS_VISTA_X86:
        {
            CLog::getInstance()->logDebug("......创建CWinVistaX86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWinVistaX86DiskScanConfig();
        }
        case WINDOWS_VISTA_X64:
        {
            CLog::getInstance()->logDebug("......创建CWinVistaX64DiskScanConfig，初始化垃圾扫描信息......");
            return new CWinVistaX64DiskScanConfig();
        }
        case WINDOWS_7_X86:
        {
            CLog::getInstance()->logDebug("......创建CWin7X86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin7X86DiskScanConfig();
        }
        case WINDOWS_7_X64:
        {
            CLog::getInstance()->logDebug("......创建CWin7X64DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin7X64DiskScanConfig();
        }
        case WINDOWS_8_X86:
        {
            CLog::getInstance()->logDebug("......创建CWin8X86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin8X86DiskScanConfig();
        }
        case WINDOWS_8_X64:
        {
            CLog::getInstance()->logDebug("......创建CWin8X64DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin8X64DiskScanConfig();
        }
        case WINDOWS_81_X86:
        {
            CLog::getInstance()->logDebug("......创建CWin81X86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin81X86DiskScanConfig();
        }
        case WINDOWS_81_X64:
        {
            CLog::getInstance()->logDebug("......创建CWin81X64DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin81X64DiskScanConfig();
        }
        case WINDOWS_10_X86:
        {
            CLog::getInstance()->logDebug("......创建CWin10X86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin10X86DiskScanConfig();
        }
        case WINDOWS_10_X64:
        {
            CLog::getInstance()->logDebug("......创建CWin10X64DiskScanConfig，初始化垃圾扫描信息......");
            return new CWin10X64DiskScanConfig();
        }
        default:
        {
            CLog::getInstance()->logDebug("......未知系统，使用默认，创建CWinXpX86DiskScanConfig，初始化垃圾扫描信息......");
            return new CWinXpX86DiskScanConfig();
        }
    }
}

bool CDiskScanConfig::loadDiskScanConfigFromDb(QString sSql)
{
    CLog::getInstance()->logDebug("......CDiskScanConfig::loadDiskScanConfigFromDb......");
    if(!m_dbConnect.open())
    {
        qDebug() << "database open fail.";
        CLog::getInstance()->logDebug("......database open fail......");
        QSqlError err = m_dbConnect.lastError();
        CLog::getInstance()->logDebug(err.text().toStdString().c_str());
        return false;
    }

    QSqlQuery query(sSql);
    QSqlError err = query.lastError();
    if(0 == err.type())
    {
        TDiskScanInfo* pDiskScanInfo;
        while(query.next())
        {
            pDiskScanInfo = new TDiskScanInfo();
            pDiskScanInfo->platform_id = query.value("platform_id").toInt();
            pDiskScanInfo->platform_name = query.value("platform_name").toString();
            pDiskScanInfo->platform_cpu = query.value("platform_cpu").toInt();
            pDiskScanInfo->desc_cn = query.value("desc_cn").toString();
            pDiskScanInfo->desc_en = query.value("desc_en").toString();
            pDiskScanInfo->trash_path = query.value("trash_path").toString();
            pDiskScanInfo->ext_name = query.value("ext_name").toString();
            pDiskScanInfo->clean_all_flag = query.value("clean_all_flag").toInt();
            m_lstDiskScanInfo.append(pDiskScanInfo);
        }
        m_dbConnect.close();
        return true;
    }
    else
    {
        qDebug() << "sql exec fail.";
        qDebug() << err.text();
        CLog::getInstance()->logDebug(err.text().toStdString().c_str());
        m_dbConnect.close();
        return false;
    }
}

bool CDiskScanConfig::loadRegScanConfigFromDb(QString sSql)
{
    CLog::getInstance()->logDebug("......CDiskScanConfig::loadRegScanConfigFromDb......");
    if(!m_dbConnect.open())
    {
        qDebug() << "database open fail.";
        CLog::getInstance()->logDebug("......database open fail......");
        return false;
    }

    QSqlQuery query(sSql);
    QSqlError err = query.lastError();
    if(0 == err.type())
    {
        TRegScanInfo* pRegScanInfo;
        while(query.next())
        {
            pRegScanInfo = new TRegScanInfo();
            pRegScanInfo->platform_id = query.value("platform_id").toInt();
            pRegScanInfo->platform_name = query.value("platform_name").toString();
            pRegScanInfo->platform_cpu = query.value("platform_cpu").toInt();
            pRegScanInfo->err_type = query.value("err_type").toInt();
            pRegScanInfo->desc_cn = query.value("desc_cn").toString();
            pRegScanInfo->desc_en = query.value("desc_en").toString();
            pRegScanInfo->reg_path = query.value("reg_path").toString();
            m_lstRegScanInfo.append(pRegScanInfo);
        }
        m_dbConnect.close();
        return true;
    }
    else
    {
        qDebug() << "sql exec fail.";
        qDebug() << err.text();
        CLog::getInstance()->logDebug("SQL执行错误：");
        CLog::getInstance()->logDebug(err.text().toStdString().c_str());
        m_dbConnect.close();
        return false;
    }
}



