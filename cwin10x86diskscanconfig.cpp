#include "cwin10x86diskscanconfig.h"

#include <QList>

CWin10X86DiskScanConfig::CWin10X86DiskScanConfig()
{

}

//返回需要扫描的目录信息列表
QList<TDiskScanInfo*>* CWin10X86DiskScanConfig::getDiskScanInfo()
{
    //已经加载过了，则直接返回
    if(!m_lstDiskScanInfo.isEmpty())
    {
        return &m_lstDiskScanInfo;
    }

    QString sSql = "select platform_id,platform_name,platform_cpu,"
                   " desc_cn,desc_en,trash_path,ext_name,clean_all_flag "
                   " from disk_scan_info where platform_id=100 and platform_cpu=0";

    if(loadConfigFromDb(sSql))
    {
        return &m_lstDiskScanInfo;
    }
    else
    {
        return NULL;
    }
}



