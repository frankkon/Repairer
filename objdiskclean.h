#ifndef OBJDISKCLEAN_H
#define OBJDISKCLEAN_H

#include <QObject>
#include <cleanitem.h>

class ObjDiskClean : public QObject
{

    Q_OBJECT
public:
    explicit ObjDiskClean(QObject *parent = 0);
    ~ObjDiskClean();

    void addCleanSuffix(QString strSuffix);
    void removeCleanSuffix(QString strSuffix);

    void addCleanFile(QString strFile);
    void removeCleanFile(QString strFile);


private:
    void findFiles(QString strPath);
    void procFile(QString strFilePath);  //判断是否删除该文件

    CleanItem* addCleanItem(int nType, QString strName, QString strFilePath);
    void clearAllItems();

signals:
    void sigUpdateCurrentCheckFile(bool bClean, QString strFilePath, CleanItem* pItem);
    void sigCleanFinish();

public slots:
    void clean();

private:
    QList<QString> m_lstCleanSuffix;
    QList<QString> m_lstCleanFile;

public:
    QList<CleanItem*> m_lstCleanItems;
};

#endif // OBJDISKCLEAN_H
