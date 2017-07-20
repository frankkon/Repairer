#ifndef CWINXPX64DISKSCANCONFIG_H
#define CWINXPX64DISKSCANCONFIG_H

#include "cdiskscanconfig.h"


class CWinXpX64DiskScanConfig : public CDiskScanConfig
{
public:
    CWinXpX64DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWINXPX64DISKSCANCONFIG_H
