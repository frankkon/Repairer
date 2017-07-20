#ifndef CWINXPX86DISKSCANCONFIG_H
#define CWINXPX86DISKSCANCONFIG_H

#include "cdiskscanconfig.h"


class CWinXpX86DiskScanConfig : public CDiskScanConfig
{
public:
    CWinXpX86DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWINXPX86DISKSCANCONFIG_H
