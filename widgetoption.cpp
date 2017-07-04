#include "widgetoption.h"
#include "ui_widgetoption.h"
#include "global.h"
#include <QSettings>

WidgetOption::WidgetOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetOption)
{
    ui->setupUi(this);

    QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings reg(path, QSettings::NativeFormat);
    QString strValue = reg.value(Global::s_strExeName).toString();
    if(strValue.size() > 3)
    {
        ui->checkBox_startup->setChecked(true);
    }
    else
    {
        ui->checkBox_startup->setChecked(false);
    }
    connect( ui->pushButton_apply, &QPushButton::clicked, this, &WidgetOption::apply );

}

WidgetOption::~WidgetOption()
{
    delete ui;
}

void WidgetOption::apply()
{

    if( ui->checkBox_startup->isChecked() )
    {
        QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        QSettings reg(path, QSettings::NativeFormat);
        QString strValue = QString("%1 -hide").arg(Global::s_strExePath);
        reg.setValue(Global::s_strExeName, strValue);
    }
    else
    {
        QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        QSettings reg(path, QSettings::NativeFormat);
        reg.setValue(Global::s_strExeName, "");
    }
}
