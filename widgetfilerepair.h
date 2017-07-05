#ifndef WIDGETFILEREPAIR_H
#define WIDGETFILEREPAIR_H


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

#include <objfilerepair.h>
#include <filelistdownload.h>

namespace Ui
{
    class WidgetFileRepair;
}

class WidgetFileRepair : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetFileRepair(QWidget* parent = 0);
    ~WidgetFileRepair();

    void translate();

    void init();

private:
    void initControl();
    void initLayout();

signals:
    void sigDownloadFile(QString strFile);


public slots:
    void downloadFile();
    void downloadFinished(FileListDownload* pDownload, bool bSuccess);
    void deleteFile();
    void openFile();
    void updateDownload(int nPercentage);

private:
    Ui::WidgetFileRepair* ui;

    //    QLabel*         m_pLabIco;
    //    QLabel*         m_pLabTitleEdt;
    //    QLabel*         m_pLabTitleLst;
    //    QLineEdit*      m_pEdtFile;
    //    QPushButton*    m_pBtnDownload;
    //    QTableWidget*   m_pTableWidget;

    ObjFileRepair*  m_pObjFileRepair;
    QThread* m_pThread;

    bool m_bDownload;  //正在下载文件


};

#endif // WIDGETFILEREPAIR_H
