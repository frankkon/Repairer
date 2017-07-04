#include "widgetunregister.h"
#include "ui_widgetunregister.h"

#include "global.h"
#include "tool.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

WidgetUnregister::WidgetUnregister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidgetUnregister)
{
    ui->setupUi(this);

    connect( ui->pushButton_unregister, &QPushButton::clicked, this, &WidgetUnregister::unregister );
    connect( ui->pushButton_close, &QPushButton::clicked, this, &WidgetUnregister::close );
}

WidgetUnregister::~WidgetUnregister()
{
    delete ui;
}

void WidgetUnregister::setLabelText( QString str )
{
    ui->label->setText(str);
}

void WidgetUnregister::unregister()
{
    if( QMessageBox::No == QMessageBox::question( this, Global::s_strExeName, "If you are unhappy with your purchase, for any reason, simply contact us via e-mail for a full refund.\ndllescort.com@gmail.com" ))
    {
        return;
    }


    QString strPath = QString("%1\\dllescort\\data.dat").arg(Global::s_strProgramDataPath);

    if(!Tool::isFileOrDirExist(strPath))
    {
        Tool::createDirectory(strPath,true);
    }

    QFile file(strPath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"open file "<<strPath<<" failed,error:"<<file.errorString();
    }
    else
    {

    }

    QTextStream txtInput(&file);

    bool bNew = true;
    bool bNoLine = true;
    while(!txtInput.atEnd())
    {
        QString strLine = txtInput.readLine();
        if( strLine == Global::s_seriesNumber.strEndode )
        {
            bNew = false;
            break;
        }
        bNoLine = false;
    }

    if(bNew)
    {
        if(!bNoLine)
        {
            txtInput<<"\r\n";
        }

        txtInput<<Global::s_seriesNumber.strEndode;
    }

    file.close();

    Global::s_bActive = false;

    QDialog::close();

    QMessageBox::about( this, Global::s_strExeName, "Please contact us for sending mail dllescort.com@gmail.com" );
}
