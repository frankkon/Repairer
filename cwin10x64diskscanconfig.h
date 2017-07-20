#ifndef CWIN10X64DISKSCANCONFIG_H
#define CWIN10X64DISKSCANCONFIG_H

#include "cdiskscanconfig.h"


class CWin10X64DiskScanConfig : public CDiskScanConfig
{
public:
    CWin10X64DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWIN10X64DISKSCANCONFIG_H
