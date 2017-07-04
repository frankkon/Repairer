#include "dlgregisterkey.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QDesktopWidget>

DlgRegisterKey::DlgRegisterKey(QWidget *parent) :
    DialogDrop(parent)
{
    m_pWidgetRegister = new WidgetRegister();

    setFixedSize( 780, 559 );

    QDesktopWidget *deskdop = QApplication::desktop();
    move((deskdop->width() - this->width())/2, (deskdop->height() - this->height())/2);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->addWidget(m_pWidgetRegister);
    setLayout(layout);
}
