#ifndef CDISKSCANCONFIG_H
#define CDISKSCANCONFIG_H

#include <QList>

class QString;

#define CONFIG_DB_NAME ("repairer.db")

enum EPlatform
{
    WINDOWS_XP_X86 = 510,
    WINDOWS_XP_X64 = 521,
    WINDOWS_VISTA_X86 = 600,
    WINDOWS_VISTA_X64 = 601,
    WINDOWS_7_X86 = 610,
    WINDOWS_7_X64 = 611,
    WINDOWS_8_X86 = 620,
    WINDOWS_8_X64 = 621,
    WINDOWS_81_X86 = 630,
    WINDOWS_81_X64 = 631,
    WINDOWS_10_X86 = 1000,
    WINDOWS_10_X64 = 1001
};

struct TDiskScanInfo
{
    int platform_id;
    QString platform_name;
    int platform_cpu;
    QString desc_cn;
    QString desc_en;
    QString trash_path;
    QString ext_name;
    int clean_all_flag;
    qint64 total_size;  //扫描出的该类型一共占用的磁盘空间
    int file_count;    //扫描出的该类型文件总数
    QStringList fileList; //扫描出的该类型文件的列表
};

class CDiskScanConfig
{
public:
    CDiskScanConfig();
    virtual ~CDiskScanConfig();

    //根据不同的Windows版本，创建对应的config类对象
    static CDiskScanConfig* loadConfig();

    //返回需要扫描的目录信息列表
    virtual QList<TDiskScanInfo*>* getDiskScanInfo() = 0;

protected:
    bool loadConfigFromDb(QString sSql);

protected:
    QList<TDiskScanInfo*> m_lstDiskScanInfo;
};

#endif // CDISKSCANCONFIG_H
