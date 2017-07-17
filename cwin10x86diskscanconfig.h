#ifndef CWIN10X86DISKSCANCONFIG_H
#define CWIN10X86DISKSCANCONFIG_H

#include "cdiskscanconfig.h"

class CWin10X86DiskScanConfig : public CDiskScanConfig
{
public:
    CWin10X86DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
};

#endif // CWIN10X86DISKSCANCONFIG_H
