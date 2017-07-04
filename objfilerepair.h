#ifndef OBJFILEREPAIR_H
#define OBJFILEREPAIR_H

#include <QObject>
#include <QList>
#include <filelistdownload.h>
#include "http.h"

class ObjFileRepair : public QObject
{
    Q_OBJECT
public:
    explicit ObjFileRepair(QObject *parent = 0);
    ~ObjFileRepair();

    void deleteDownloadFile(int nIndex);

    void init();

    void timerEvent(QTimerEvent *event);

private:
    QString getFileRepairUrl(QString strFile);
    QString getDownloadFilePath(QString strName);


signals:
    void sigDownloadFinished( FileListDownload* pDownload, bool bSuccess );  //下载完毕通知UI
    void sigUpdateDownload( int nPercentage );

public slots:
    void downloadFile( QString strFile );
    void downloadFinished( QString strFile, bool bSuccess );
    void CurrentDownloadFile( int nPercentage );
    void openFile( int nIndex );

private:
    QList<FileListDownload*> m_lstFiles;
    Http* m_pHttp;

    QString m_strFilePathRepair;
    QString m_strFileDownloadName;

};

#endif // OBJFILEREPAIR_H
