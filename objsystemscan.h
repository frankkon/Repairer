#ifndef OBJSYSTEMSCAN_H
#define OBJSYSTEMSCAN_H

#include <QObject>
#include <QStringList>
#include <QMap>

#include "global.h"
#include "filelistitem.h"

struct CommonFiles
{
    QList<FileListItem*> lstFiles;
    QString strSuffix;
};


class ObjSystemScan : public QObject
{
    Q_OBJECT
public:
    explicit ObjSystemScan(QObject *parent = 0);
    ~ObjSystemScan();

    void deleteFromTable( int nIndex );
    FileListItem* GetLostFileFromList( int nIndex );

    void ReadDatFile();
    void clearLostFile();

private:
    void generateDatFileName();

    void CheckSystemFile();

    void addLostFile(FileListItem* pItem);

    bool isFileNotCheck( QString strFile );


signals:
    void sigCurrentCheckFile( QString strFile, int nTotal, int nCurrent );
    void sigAddFileToTable( FileListItem* pItem );
    void sigDownloadFinished( FileListItem* pItem, bool bSuccess );
    void sigCurrentDownloadFile( FileListItem* pItem, int nPercentage );

public slots:
    void startCheckDlls();
    void pauseCheckDlls();
    void quitCheckDlls();
    void downloadFile( int nIndex );
    void downloadFinished(FileListItem* pItem, bool bSuccess);
    void currentDownloadFile( FileListItem* pItem, int nPercentage );

public:
    QList<FileListItem*> m_lstFileDownloadTmp;  //下载用的，临时
    int m_nCurIndex;
private:
    QString m_strSystemPath;

    QString m_strDatfile;
    QStringList m_lstFileTotal;             //核查的文件
    QList<FileListItem*> m_lstFileLost;     //丢失的文件
    QMap< QString, CommonFiles > m_mapFile;

    bool m_bChecked;
    bool m_bDownload;

    int m_nFixedCount;
};

#endif // OBJSYSTEMSCAN_H
