#ifndef FILELISTITEM_H
#define FILELISTITEM_H

#include <QPushButton>
#include <QProgressBar>
#include <QObject>
#include <QLabel>
#include "http.h"

enum E_STATUS
{
    E_STATUS_DISPLAY,
    E_STATUS_DOWNLOADING,
    E_STATUS_LOCAL,
};

class FileListItem : public QObject
{
    Q_OBJECT
public:
    explicit FileListItem(QObject *parent = 0);
    ~FileListItem();

    void openFile();
    void cancelDownload();

    void setThread( QThread* pThread );
    void setDownloadButton( QPushButton* pBtn );
    void setDeleteButton( QPushButton* pBtn );

signals:
    void sigCurrentDownloadFile( FileListItem* pItem, int nPercentage );
    void sigDownloadFinished( FileListItem* pItem, bool bSuccess );

public slots:
    void CurrentDownloadFile( int nPercentage );
    void downloadFinished( QString strFile, bool bSuccess );

    void downloadFile();

public:
    static QString s_strFilePathSysScan;

    QPushButton* m_pBtnDownload;
    QPushButton* m_pBtnDelete;
    QLabel* m_pLabel;
    QProgressBar* m_pPbr;
    Http*    m_pHttp;

    QString m_strFileName;       //文件名+后缀 (adsldp.dll)
    QString m_strFilePath;       //文件应该存在的目录 ()
    QString m_strFileDownload;   //下载后保存的目录
    QString m_strFileUrl;        //服务器下载url

    QString m_strDownloadError;  //下载错误时给出的提示

    E_STATUS m_eStatus;

    int m_nIndex;

};

#endif // FILELISTITEM_H
