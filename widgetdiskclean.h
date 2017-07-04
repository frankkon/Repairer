#ifndef WIDGETDISKCLEAN_H
#define WIDGETDISKCLEAN_H

#include <QWidget>
#include <QPushButton>
#include "objdiskclean.h"

namespace Ui {
class WidgetDiskClean;
}

class WidgetDiskClean : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDiskClean(QWidget *parent = 0);
    ~WidgetDiskClean();

signals:
    void sigClean();

public slots:
    void updateCurrentCheckFile(bool bClean, QString strFilePath, CleanItem* pItem);
    void cleanFinish();

private:
    Ui::WidgetDiskClean *ui;


    ObjDiskClean* m_pObjDiskClean;
    QThread* m_pThread;
};

#endif // WIDGETDISKCLEAN_H
