#include "cleanitem.h"


CleanItem::CleanItem(QObject *parent) :
    QObject(parent)
{
    m_pCheckBox = NULL;
}

CleanItem::~CleanItem()
{
    if(m_pCheckBox)
    {
        delete m_pCheckBox;
        m_pCheckBox = NULL;
    }

    for(QList<CleanItem*>::iterator it = m_lstChild.begin(); it != m_lstChild.end(); it++)
    {
        CleanItem* pTmp = *it;
        delete pTmp;
        pTmp = NULL;
    }
    m_lstChild.clear();
}

void CleanItem::check()
{

}
