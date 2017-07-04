#ifndef WIDGETVERSION_H
#define WIDGETVERSION_H

#include "dialogdrop.h"
#include "filelistdownload.h"

namespace Ui {
class WidgetVersion;
}

class WidgetVersion : public DialogDrop
{
    Q_OBJECT

public:
    explicit WidgetVersion( FileListDownload* pFileList, QWidget *parent = 0 );
    ~WidgetVersion();

private:
    Ui::WidgetVersion *ui;
};

#endif // WIDGETVERSION_H
