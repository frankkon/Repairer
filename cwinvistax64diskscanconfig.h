#ifndef CWINVISTAX64DISKSCANCONFIG_H
#define CWINVISTAX64DISKSCANCONFIG_H

#include "cdiskscanconfig.h"


class CWinVistaX64DiskScanConfig : public CDiskScanConfig
{
public:
    CWinVistaX64DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWINVISTAX64DISKSCANCONFIG_H
