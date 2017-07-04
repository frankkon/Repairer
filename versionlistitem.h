#ifndef VERSIONLISTITEM_H
#define VERSIONLISTITEM_H

#include <QObject>

class VersionListItem : public QObject
{
    Q_OBJECT
public:
    explicit VersionListItem(QObject *parent = 0);

    static bool isGreaterVersion( QString sV1, QString sV2 );  //V1的版本号大 返回true 否则返回false

    bool isGreaterVersion( VersionListItem* p );

signals:

public slots:
    void open();

public:
    bool m_bEmpty;
    QString m_strFilePath;
    QString m_strFileVersion;
    bool  m_bX86;
    float m_fSizeKb;

};

#endif // VERSIONLISTITEM_H
