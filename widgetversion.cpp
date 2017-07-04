#include "widgetversion.h"
#include "ui_widgetversion.h"

WidgetVersion::WidgetVersion( FileListDownload* pFileList, QWidget *parent ) :
    DialogDrop(parent),
    ui(new Ui::WidgetVersion)
{
    ui->setupUi(this);

    ui->label_Name->setText(pFileList->m_strFileName);


}

WidgetVersion::~WidgetVersion()
{
    delete ui;
}
