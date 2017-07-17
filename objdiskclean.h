#ifndef OBJDISKCLEAN_H
#define OBJDISKCLEAN_H

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
//    void sigUpdateCurrentCheckFile(bool bClean, QString strFilePath, CleanItem* pItem);

    //通知界面更新当前扫描进展
    void sigNotifyUIUpdateScanProgress(qint64 fileCount, QString fileDesc, QString currentFile);

    //通知界面更新当前清理进展
    void sigNotifyUIUpdateCleanProgress(qint64 fileCount, QString fileDesc, QString currentFile);

    //通知界面完成所有错误扫描
    void sigNotifyUIScanFinished(qint64 iTotalFileErrors, qint64 iTotalRegErrors);

    //通知界面完成所有错误清理
    void sigNotifyUICleanFinished();

public slots:
    //扫描所有注册表错误和磁盘垃圾
    void scanAllErrors();

    //清理所有注册表错误和磁盘垃圾
    void cleanAllErrors();
    
public:
//    QList<CleanItem*> m_lstNeedToBeCleanedItems;

private:
    void scanAllTrashFiles();
    void scanAllRegErrors();
    void cleanAllTrashFiles();
    void cleanAllRegErrors();
    void scanTrashFilesInPath(TDiskScanInfo* scanInfo);

    //替换目录中的环境变量
    void replaceEnv(QString& srcDir);


private:
//    QList<QString> m_lstTrashFileSuffix;
//    QList<QString> m_lstNeedToBeCleanedFiles;
    CDiskScanConfig* m_pDiskScanConfig;
    qint64 m_iTotalScanedFiles;
    qint64 m_iTotalScanedRegs;

};

#endif // OBJDISKCLEAN_H
