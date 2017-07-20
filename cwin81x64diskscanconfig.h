#ifndef CWIN81X64DISKSCANCONFIG_H
#define CWIN81X64DISKSCANCONFIG_H


#include "cdiskscanconfig.h"

class CWin81X64DiskScanConfig : public CDiskScanConfig
{
public:
    CWin81X64DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWIN81X64DISKSCANCONFIG_H
