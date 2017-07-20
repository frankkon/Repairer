#ifndef CWIN8X86DISKSCANCONFIG_H
#define CWIN8X86DISKSCANCONFIG_H


#include "cdiskscanconfig.h"

class CWin8X86DiskScanConfig : public CDiskScanConfig
{
public:
    CWin8X86DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWIN8X86DISKSCANCONFIG_H
