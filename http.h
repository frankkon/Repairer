#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QtNetwork>

class  Http : public QObject
{
    Q_OBJECT
public:
    explicit Http(QObject *parent = 0);
    ~Http();

    void downloadFile( QString strUrl, QString strSavePath );
    void cancelDownload();

    qint64 getFileSize(QString url ,int tryTimes); //请求的文件服务器不存在，返回-1

signals:
    void sigCurrentDownloadFile( int nPercentage );
    void sigDownloadFinished( QString strFile, bool bSuccess );

public slots:

    void downloadError(QNetworkReply::NetworkError code);
    void downloadFinished();
    void downloadReadyRead();
    void updateDataReadProgress( qint64 bytesReceived, qint64 bytesTotal );

    void onDownloadTimer();

private:
    void tryDownloadAgain();

    void timerEvent(QTimerEvent* event);

private:
    static QNetworkAccessManager*  s_pNetMgr;
    QNetworkReply*          m_pNetReply;
    QFile*                  m_pFile;
    bool                    m_bSuccess;
    QString                 m_strFileName;
    QString                 m_strFileNameTmp;
    QString        m_strUrl;

    qint64 m_nFileSize;
    int            m_nTryTotal;
    int            m_nTryCur;
    qint64            m_nCurDownload;
    qint64         m_nLastDownload;

    QTimer         m_timer;
    int m_nTimerId;

    int m_nTimerInterval;

    int m_nNoData;
    bool m_bAbort;
    int n_reConn;
};

#endif // HTTP_H
