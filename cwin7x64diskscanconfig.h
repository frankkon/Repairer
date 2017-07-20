#ifndef CWIN7X64DISKSCANCONFIG_H
#define CWIN7X64DISKSCANCONFIG_H


#include "cdiskscanconfig.h"

class CWin7X64DiskScanConfig : public CDiskScanConfig
{
public:
    CWin7X64DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWIN7X64DISKSCANCONFIG_H
