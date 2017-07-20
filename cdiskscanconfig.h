#ifndef CDISKSCANCONFIG_H
#define CDISKSCANCONFIG_H

#include <QList>
#include <QtSql/QSqlDatabase>

class QSqlDatabase;

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
    double total_size;  //扫描出的该类型一共占用的磁盘空间
    int file_count;    //扫描出的该类型文件总数
    QStringList file_list; //扫描出的该类型文件的列表
};

struct TRegScanInfo
{
    int platform_id;
    QString platform_name;
    int platform_cpu;
    int err_type;
    QString desc_cn;
    QString desc_en;
    QString reg_path;
    int err_count;    //扫描出的该类型错误总数
    QStringList err_list; //扫描出的该类型错误的列表
};

enum ERegErrorType
{
    DLL_ERROR = 1, //错误的DLL
    APP_COMPAT = 2, //程序兼容性选项信息
    INVALID_STARTUP = 3, //无效的启动项
    RESIDUAL_UNINSTALL = 4, //残留的软件卸载信息
    WRONG_HELP = 5,  //错误的帮助信息
    APP_INSTALL = 6,  //错误的安装信息
    APP_PATH = 7,        //错误应用程序路径
    RESIDUAL_INSTALL = 8,  //残留的软件安装
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
    virtual QList<TRegScanInfo*>* getRegScanInfo() = 0;

protected:
    bool loadDiskScanConfigFromDb(QString sSql);
    bool loadRegScanConfigFromDb(QString sSql);

protected:
    QSqlDatabase m_dbConnect;
    QList<TDiskScanInfo*> m_lstDiskScanInfo;
    QList<TRegScanInfo*> m_lstRegScanInfo;
};

#endif // CDISKSCANCONFIG_H
