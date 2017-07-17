#include "windows.h"

#include "global.h"
#include "cdiskscanconfig.h"
#include "cwin10x86diskscanconfig.h"


#include <QtSql/QSqlDatabase>
#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>
#include <QString>
#include <QDebug>

CDiskScanConfig::CDiskScanConfig()
{
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

bool CDiskScanConfig::loadConfigFromDb(QString sSql)
{
    QSqlDatabase dbConnect = QSqlDatabase::addDatabase("QSQLITE");
    dbConnect.setDatabaseName(CONFIG_DB_NAME);
    if(!dbConnect.open())
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
        dbConnect.close();
        return true;
    }
    else
    {
        qDebug() << "sql exec fail.";
        qDebug() << err.text();
        dbConnect.close();
        return false;
    }
}



