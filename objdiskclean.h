#ifndef OBJDISKCLEAN_H
#define OBJDISKCLEAN_H

#include "windows.h"

#include <QObject>
#include <cleanitem.h>
#include "cdiskscanconfig.h"

class ObjDiskClean : public QObject
{

    Q_OBJECT
public:
    explicit ObjDiskClean(QObject* parent = 0);
    ~ObjDiskClean();

signals:
    //通知界面更新当前扫描进展
    void sigNotifyUIUpdateScanProgress(qint64 fileCount, QString fileDesc, QString currentFile);

    //通知界面更新当前清理进展
    void sigNotifyUIUpdateCleanProgress(qint64 fileCount, QString fileDesc, QString currentFile);

    //通知界面完成所有错误扫描
    void sigNotifyUIScanFinished(qint64 iTotalFileErrors, double iTotalFileSize, qint64 iTotalRegErrors);

    //通知界面完成所有错误清理
    void sigNotifyUICleanFinished();

public slots:
    //扫描所有注册表错误和磁盘垃圾
    void scanAllErrors();

    //清理所有注册表错误和磁盘垃圾
    void cleanAllErrors();
    
private:
    void scanAllTrashFiles();
    void scanAllRegErrors();
    void cleanAllTrashFiles();
    void cleanAllRegErrors();
    void scanTrashFilesInPath(TDiskScanInfo* scanInfo);
    void scanRegInPath(TRegScanInfo* scanInfo);

    //按照不同的注册表错误类型进行不同的扫描
    void scanRegByDllErrors(TRegScanInfo* scanInfo);
    void scanRegByAppCompat(TRegScanInfo* scanInfo);
    void scanRegByInvalidStartup(TRegScanInfo* scanInfo);
    void scanRegByResidualUninstall(TRegScanInfo* scanInfo);
    void scanRegByWrongHelp(TRegScanInfo* scanInfo);
    void scanRegByAppInstall(TRegScanInfo* scanInfo);
    void scanRegByAppPath(TRegScanInfo* scanInfo);
    void scanRegByResidualInstall(TRegScanInfo* scanInfo);

    //将键值字符串转换为HKEY
    HKEY getPrimaryKeyHandle(QString sKey);

    //替换目录中的环境变量
    void replaceEnv(QString& srcDir);

#ifndef QT_NO_DEBUG
    void dumpScanFile();
    void dumpScanReg();
#endif


private:
    //初始化扫描的垃圾目录和注册表路径
    CDiskScanConfig* m_pDiskScanConfig;

    //扫描到的垃圾文件计数
    qint64 m_iTotalScanedFiles;

    //扫描到的垃圾文件大小总计
    double m_dTotalScanedFileSize;

    //扫描到的注册表错误计数
    qint64 m_iTotalScanedRegs;

};

#endif // OBJDISKCLEAN_H
