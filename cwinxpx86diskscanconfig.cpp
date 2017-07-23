#include "cwinxpx86diskscanconfig.h"
#include "Log.h"

CWinXpX86DiskScanConfig::CWinXpX86DiskScanConfig()
{

}

//返回需要扫描的目录信息列表
QList<TDiskScanInfo*>* CWinXpX86DiskScanConfig::getDiskScanInfo()
{
    //已经加载过了，则直接返回
    if(!m_lstDiskScanInfo.isEmpty())
    {
        return &m_lstDiskScanInfo;
    }

    QString sSql = "select platform_id,platform_name,platform_cpu,"
                   " desc_cn,desc_en,trash_path,ext_name,clean_all_flag "
                   " from disk_scan_info where platform_id=51 and platform_cpu=0";

    if(loadDiskScanConfigFromDb(sSql))
    {
        return &m_lstDiskScanInfo;
    }
    else
    {
        return NULL;
    }
}

QList<TRegScanInfo*>* CWinXpX86DiskScanConfig::getRegScanInfo()
{
    //已经加载过了，则直接返回
    CLog::getInstance()->logDebug("......CWinXpX86DiskScanConfig::getRegScanInfo()......");
    if(!m_lstRegScanInfo.isEmpty())
    {
        return &m_lstRegScanInfo;
    }

    QString sSql = "select platform_id,platform_name,platform_cpu,"
                   " err_type,desc_cn,desc_en,reg_path "
                   " from reg_scan_info where platform_id=51 and platform_cpu=0";

    if(loadRegScanConfigFromDb(sSql))
    {
        CLog::getInstance()->logDebug("......loadRegScanConfigFromDb()---OK......");
        return &m_lstRegScanInfo;
    }
    else
    {
        CLog::getInstance()->logDebug("......loadRegScanConfigFromDb()---Fail......");
        return NULL;
    }
}
