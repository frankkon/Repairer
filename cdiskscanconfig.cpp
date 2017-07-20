#include "windows.h"

#include "global.h"
#include "cdiskscanconfig.h"
#include "cwin10x86diskscanconfig.h"


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
            return NULL;
        }
        case WINDOWS_XP_X64:
        {
            return NULL;
        }
        case WINDOWS_VISTA_X86:
        {
            return NULL;
        }
        case WINDOWS_VISTA_X64:
        {
            return NULL;
        }
        case WINDOWS_7_X86:
        {
            return NULL;
        }
        case WINDOWS_7_X64:
        {
            return NULL;
        }
        case WINDOWS_8_X86:
        {
            return NULL;
        }
        case WINDOWS_8_X64:
        {
            return NULL;
        }
        case WINDOWS_81_X86:
        {
            return NULL;
        }
        case WINDOWS_81_X64:
        {
            return NULL;
        }
        case WINDOWS_10_X86:
        {
            return new CWin10X86DiskScanConfig();
        }
        case WINDOWS_10_X64:
        {
            return NULL;
        }
        default:
        {
            return NULL;
        }
    }
}

bool CDiskScanConfig::loadDiskScanConfigFromDb(QString sSql)
{
    if(!m_dbConnect.open())
    {
        qDebug() << "database open fail.";
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
        m_dbConnect.close();
        return false;
    }
}

bool CDiskScanConfig::loadRegScanConfigFromDb(QString sSql)
{
    if(!m_dbConnect.open())
    {
        qDebug() << "database open fail.";
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
        m_dbConnect.close();
        return false;
    }
}



