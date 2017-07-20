#ifndef CWIN81X86DISKSCANCONFIG_H
#define CWIN81X86DISKSCANCONFIG_H


#include "cdiskscanconfig.h"

class CWin81X86DiskScanConfig : public CDiskScanConfig
{
public:
    CWin81X86DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWIN81X86DISKSCANCONFIG_H
