#ifndef FILELISTDOWNLOAD_H
#define FILELISTDOWNLOAD_H

#include <QObject>
#include <QVector>

#include "versionlistitem.h"

class FileListDownload : public QObject
{
    Q_OBJECT
public:
    explicit FileListDownload(QObject* parent = 0);

    void generateVersionList(QString strPathTmp);
    bool loadVersionList();  //成功读取 返回true 没有文件 返回false

    void open();

signals:

public slots:
    void deleteFile();


private:
    void addVersionItem(QVector<VersionListItem*>& vct, VersionListItem* pItem);

public:

    QVector<VersionListItem*> m_vctVersionX86;
    QVector<VersionListItem*> m_vctVersionX64;
    QString m_strPath;
    QString m_strFileName;
    QString m_strError;

};

#endif // FILELISTDOWNLOAD_H
