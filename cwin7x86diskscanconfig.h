#ifndef CWIN7X86DISKSCANCONFIG_H
#define CWIN7X86DISKSCANCONFIG_H


#include "cdiskscanconfig.h"

class CWin7X86DiskScanConfig : public CDiskScanConfig
{
public:
    CWin7X86DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWIN7X86DISKSCANCONFIG_H
