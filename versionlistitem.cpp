#include "versionlistitem.h"
#include "tool.h"
#include "dlgregisterkey.h"

VersionListItem::VersionListItem(QObject *parent) :
    QObject(parent)
{
    m_fSizeKb = 0.f;
    m_bX86 = true;
    m_bEmpty = true;
}

bool VersionListItem::isGreaterVersion( QString sV1, QString sV2 )
{
    bool bV1Greater = false;
    //int nTmp1 = 0, nTmp2 = 0;
    while(true)
    {
        int i1 = 0, i2 = 0;
        i1 = sV1.indexOf('.');
        i2 = sV2.indexOf('.');

        QString s1 = sV1.left(i1);
        QString s2 = sV2.left(i2);

        int n1 = s1.toInt();
        int n2 = s2.toInt();

        if( n1 > n2 )
        {
            bV1Greater = true;
            break;
        }
        else if( n1 < n2 )
        {
            break;
        }

        if( i1 > 0 && i2 == -1 )
        {
            bV1Greater = true;
            break;
        }
        else if(  i1 == -1 && i2 > 0 )
        {
            break;
        }
        else if( i1 == -1 && i2 == -1 )
        {
            break;
        }

        sV1 = sV1.right(sV1.size() - i1 - 1);
        sV2 = sV2.right(sV2.size() - i2 - 1);
    }

    return bV1Greater;
}

bool VersionListItem::isGreaterVersion( VersionListItem* p )
{
    return VersionListItem::isGreaterVersion( m_strFileVersion, p->m_strFileVersion );
}

void VersionListItem::open()
{
    if(!m_bEmpty)
    {
        Tool::openAndSelectFile(m_strFilePath);
    }
    else
    {
        DlgRegisterKey dlg;
        dlg.exec();
    }

}

