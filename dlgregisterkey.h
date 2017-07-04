#ifndef DLGREGISTERKEY_H
#define DLGREGISTERKEY_H

#include "dialogdrop.h"
#include "widgetregister.h"


class DlgRegisterKey : public DialogDrop
{
    Q_OBJECT
public:
    explicit DlgRegisterKey(QWidget *parent = 0);

signals:

public slots:


private:
    WidgetRegister* m_pWidgetRegister;

};

#endif // DLGREGISTERKEY_H
