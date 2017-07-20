#ifndef CWINVISTAX86DISKSCANCONFIG_H
#define CWINVISTAX86DISKSCANCONFIG_H

#include "cdiskscanconfig.h"


class CWinVistaX86DiskScanConfig : public CDiskScanConfig
{
public:
    CWinVistaX86DiskScanConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo();
    virtual QList<TRegScanInfo*>* getRegScanInfo();
};

#endif // CWINVISTAX86DISKSCANCONFIG_H
