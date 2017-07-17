#ifndef WIDGETDISKCLEAN_H
#define WIDGETDISKCLEAN_H

#include <QWidget>
#include <QPushButton>
#include "objdiskclean.h"

namespace Ui
{
    class WidgetDiskClean;
}

class WidgetDiskClean : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDiskClean(QWidget* parent = 0);
    ~WidgetDiskClean();

signals:

public slots:
    //处理扫描过程中更新UI扫描进展信号
    void as_objDiskClean_NotifyUIUpdateScanProgress(qint64 fileCount, QString fileDesc, QString currentFile);

    //处理清理过程中更新UI清理进展信号
    void as_objDiskClean_NotifyUIUpdateCleanProgress(qint64 fileCount, QString fileDesc, QString currentFile);

    //处理完成所有错误扫描信号
    void as_objDiskClean_NotifyUIScanFinished(qint64 iTotalFileErrors, qint64 iTotalRegErrors);

    //处理完成所有错误清理信号
    void as_objDiskClean_NotifyUICleanFinished();
    

private slots:
    void on_pushButtonStartScan_clicked();
    void on_pushButtonCleanAll_clicked();

private:
    //初始化界面
    void initUI();

    //更新扫描结果到结果表中
    void appendTableItem();

    //清理结果表格
    void cleanTableItem();

    //启动扫描or清理时，加载动画
    void activateScanIcon();
    
    //完成扫描or清理时，加载静态图片
    void deactivateScanIcon();

    //扫描or清理完成后，启用按钮点击
    void enablePushButtons();
    
    //扫描or清理时，禁止按钮点击
    void disablePushButtons();


    
private:
    Ui::WidgetDiskClean* ui;

    ObjDiskClean* m_pObjDiskClean;
    QThread* m_pThread;

    //记录错误总数
    qint64 m_iTotalFileErrors;
    qint64 m_iTotalRegErrors;
};

#endif // WIDGETDISKCLEAN_H
