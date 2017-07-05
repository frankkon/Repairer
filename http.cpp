#include "http.h"
#include "tool.h"

QNetworkAccessManager* Http::s_pNetMgr;


qint64 Http::getFileSize(QString url , int tryTimes)
{
    qint64 size = -1;

    //尝试tryTimes次
    while(tryTimes --)
    {
        qDebug() << "getFileSize开始执行" << tryTimes;
        QNetworkAccessManager manager;
        QEventLoop loop;

        //发出请求，获取目标地址的头部信息
        QNetworkReply* reply = manager.head(QNetworkRequest(url));
        if(!reply) { continue; }

        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        if(reply->error() != QNetworkReply::NoError)
        {
            QString errorInfo = reply->errorString();
            qDebug() << errorInfo;

            int nIndex = errorInfo.indexOf("server replied: Not Found");
            if(nIndex > 0)  //请求的文件服务器不存在
            {
                loop.exit();
                return -1;
            }

            nIndex = errorInfo.indexOf("server replied: Forbidden");
            if(nIndex > 0)  //请求的文件服务器拒绝
            {
                loop.exit();
                return -2;
            }

            continue;
        }
        QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
        reply->deleteLater();
        size = var.toLongLong();
        break;
    }

    return size;
}

Http::Http(QObject* parent) :
    QObject(parent)
{
    Http::s_pNetMgr = NULL;
    m_bSuccess = false;

    m_nTryTotal = 3;
    m_nTryCur = 0;

    m_nCurDownload = 0;
    m_nLastDownload = 0;

    m_nTimerId = 0;

    m_nTimerInterval = 3000;

    m_strUrl = "";
    m_bAbort = false;

    n_reConn = 0;

    m_nFileSize = 0;

    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(onDownloadTimer));
}

Http::~Http()
{
    //    if(m_pFile)
    //    {
    //        m_pFile->remove();
    //    }

    if(!m_strFileNameTmp.isEmpty())
    {
        if(m_pFile)
        {

        }
        Tool::deleteFile(m_strFileNameTmp);
    }
}

void Http::downloadFile(QString strUrl, QString strSavePath)
{
    qDebug() << "Http::downloadFile() thread:" << QThread::currentThreadId();

    if(!Http::s_pNetMgr)
    {
        Http::s_pNetMgr = new QNetworkAccessManager();
    }

    QUrl url = strUrl;
    m_strUrl = strUrl;

    //    QFileInfo info(url.path());
    //    QString strFileName(info.fileName());

    //    //获取文件名
    //    if(strFileName.isEmpty())
    //    {
    //        return;
    //    }
    m_nFileSize = getFileSize(m_strUrl, 10);
    if(m_nFileSize == -1)
    {
        emit(sigCurrentDownloadFile(100));
        emit sigDownloadFinished("server replied: Not Found", false);
        return;
    }
    else if(m_nFileSize == -2)
    {
        emit(sigCurrentDownloadFile(100));
        emit sigDownloadFinished("The file name is invalid", false);
        return;
    }

    Tool::createDirectory(strSavePath, true);
    m_strFileNameTmp = strSavePath + ".tmp";
    m_strFileName = strSavePath;
    m_pFile = new QFile(m_strFileNameTmp);
    if(!m_pFile->open(QIODevice::ReadWrite))
    {
        //如果打开文件失败，则删除file，并使file指针为0，然后返回
        qDebug() << "file open error" << strSavePath;

        delete m_pFile;
        m_pFile = NULL;

        return;
    }

    //测试
    //    {
    //    QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件
    //    settings.beginGroup("Option");
    //    settings.setValue("downloadFile strUrl", strUrl );
    //    settings.endGroup();
    //    }
    m_nNoData = 0;
    m_bAbort = false;
    m_bSuccess = true;
    m_pNetReply = Http::s_pNetMgr->get(QNetworkRequest(url));

    m_nTryCur = 1;

    //下载完成后
    connect(m_pNetReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    //有可用数据时
    connect(m_pNetReply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
    //更新进度时
    connect(m_pNetReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDataReadProgress(qint64, qint64)));

    //m_timer.start(3000);
    if(m_nTimerId != 0)
    {
        killTimer(m_nTimerId);
    }
    m_nTimerId = startTimer(m_nTimerInterval);



    //connect( m_pNetReply, SIGNAL(error(QNetworkReply::NetworkError code)), this, SLOT(downloadError(QNetworkReply::NetworkError code)) );
}

void Http::tryDownloadAgain()
{
    qDebug() << "tryDownloadAgain called";

    QNetworkRequest request;
    QUrl url = m_strUrl;
    request.setUrl(url);

    if(m_pFile == NULL)
    {
        m_pFile = new QFile(m_strFileNameTmp);
        if(!m_pFile->open(QIODevice::WriteOnly))
        {
            //如果打开文件失败，则删除file，并使file指针为0，然后返回
            qDebug() << "file open error" << m_strFileNameTmp;

            delete m_pFile;
            m_pFile = NULL;

            return;
        }
    }
    m_nCurDownload = m_pFile->size();
    m_nLastDownload = m_pFile->size();
    //qint64 nTotal = getFileSize(m_strUrl, 999999999);
    //m_nFileSize = nTotal;

    QString range = QString("bytes=%0-%1").arg(m_nCurDownload).arg(m_nFileSize);
    request.setRawHeader("Range", range.toLatin1());

    m_nNoData = 0;
    m_bAbort = false;

    m_pNetReply = Http::s_pNetMgr->get(QNetworkRequest(request));

    //下载完成后
    connect(m_pNetReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    //有可用数据时
    connect(m_pNetReply, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
    //更新进度时
    connect(m_pNetReply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDataReadProgress(qint64, qint64)));

}
static int bb = 0;
void Http::updateDataReadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(-1 == bytesTotal)
    {
        //m_bSuccess = false;
        return;
    }

    //qDebug()<<bytesReceived<<bytesTotal;
    float fTmp = (float)(bytesReceived + m_nLastDownload) / (float)(bytesTotal + m_nLastDownload);
    int nPercentage = fTmp * 100;
    emit(sigCurrentDownloadFile(nPercentage));


    //    if(nPercentage > 5 && bb == 0)
    //    {
    //        m_bSuccess = false;
    //        bb = 1;
    //        downloadFinished();
    //    }
}

void Http::downloadFinished()
{
    //    if(bb == 2)
    //    {
    //        bb = 3;
    //        return;
    //    }

    if(m_pNetReply == NULL)
    {
        return;
    }

    QNetworkReply::NetworkError err = m_pNetReply->error();
    QString strError = m_pNetReply->errorString();
    qDebug() << "downloadFinished" << strError;

    if(strError == "Operation canceled")
    {
        return;
    }
    else if(strError == "Connection closed")
    {
        n_reConn++;
        if(n_reConn <= 5)
        {
            return;
        }
        else
        {
            n_reConn = 0;
            m_pFile->flush();
            m_pFile->close();
            m_pNetReply->abort();
            m_pNetReply->deleteLater();
            m_pNetReply = NULL;

            Tool::deleteFile(m_strFileNameTmp);
            downloadFile(m_strUrl, m_strFileName);
            return;
        }
    }

    if(0 != err or m_bAbort)
    {
        m_bSuccess = false;
        bb = 2;

        if(m_nTryCur <= m_nTryTotal)
        {
            m_nTryCur++;

            {
                //m_pFile->flush();
                //m_pFile->close();
                m_pNetReply->abort();

                //m_pFile->deleteLater();
                //m_pFile = NULL;
            }

            tryDownloadAgain();
            return;
        }
    }
    else
    {
        m_bSuccess = true;
    }

    if(m_pFile == NULL)
    {
        return;
    }

    if(m_pFile->size() != m_nFileSize)
    {
        m_pFile->flush();
        m_pFile->close();
        m_pNetReply->abort();
        m_pNetReply->deleteLater();
        m_pNetReply = NULL;

        Tool::deleteFile(m_strFileNameTmp);
        downloadFile(m_strUrl, m_strFileName);
        return;
    }

    QString strFile = m_pFile->fileName();
    strFile.replace("/", "\\");

    m_pFile->flush();
    m_pFile->close();
    m_pNetReply->deleteLater();
    m_pNetReply = NULL;



    if(m_bSuccess)
    {
        m_pFile->rename(m_strFileNameTmp, m_strFileName);

        emit sigDownloadFinished(m_strFileName, m_bSuccess);
    }
    else
    {
        Tool::deleteFile(m_strFileNameTmp);


        int n1 = strError.indexOf("http");
        int n2 = strError.lastIndexOf("/");
        QString tmp = strError.mid(n1, n2 - n1 + 1);
        strError.remove(tmp);

        emit sigDownloadFinished(strError, m_bSuccess);
    }



    m_strFileName.clear();
    m_strFileNameTmp.clear();
    m_strUrl.clear();

    m_nCurDownload = 0;
    m_nLastDownload = 0;
    m_nTryCur = 0;

    m_pFile->deleteLater();
    m_pFile = NULL;

    //m_timer.stop();
    killTimer(m_nTimerId);
    m_nTimerId = 0;
}

void Http::downloadReadyRead()
{

    if(m_pFile && m_pNetReply)
    {
        m_nNoData = 0;
        QByteArray ba = m_pNetReply->readAll();
        m_pFile->write(ba);
    }
}

void Http::cancelDownload()
{
    if(m_pNetReply)
    {
        m_bSuccess = false;
        m_pNetReply->abort();
    }
}

void Http::downloadError(QNetworkReply::NetworkError code)
{
    int n = 0;
}

void Http::onDownloadTimer()
{
    qDebug() << "timer call";
}

void Http::timerEvent(QTimerEvent* event)
{
    //qDebug()<<"timer call  m_nNoData:"<<m_nNoData;
    m_nNoData += m_nTimerInterval;

    if(m_nNoData > 12000)
    {
        m_bSuccess = false;
        m_bAbort = true;
        m_nNoData = 0;
        downloadFinished();
    }
}
