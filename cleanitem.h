#ifndef CLEANITEM_H
#define CLEANITEM_H



#include <QObject>
#include <QCheckBox>

class CleanItem : public QObject
{
    Q_OBJECT
public:
    explicit CleanItem(QObject *parent = 0);
    ~CleanItem();

signals:

public slots:
    void check();

public:

    int m_nType;              //0:suffix, 1:file
    QString m_strName;
    QString m_strFilePath;
    int m_nTotalSizeKb;

    QCheckBox* m_pCheckBox;

    QList<CleanItem*> m_lstChild;
};

#endif // CLEANITEM_H
