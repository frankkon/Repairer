#ifndef WIDGETSYSTEMSCAN_H
#define WIDGETSYSTEMSCAN_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTableWidget>


#include "objsystemscan.h"

namespace Ui {
class WidgetSystemScan;
}

class WidgetSystemScan : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetSystemScan(QWidget *parent = 0);
    ~WidgetSystemScan();

    void translate();
    void startCheckDlls();
    void init();

private:
    void InitControl();
    void InitLayout();

    void clearLostFile();

protected:
    void paintEvent(QPaintEvent * event);

signals:
    void sigDownloadFile( int nIndex );
    void sigStartCheckDlls();

public slots:
    void updateCurrentCheckFile( QString strFile, int nTotal, int nCurrent );
    void updateCurrentDownloadFile( FileListItem* pItem, int nPercentage );
    void downloadFinished( FileListItem* pItem, bool bSuccess );
    void addFileToTable( FileListItem* pItem );
    void deleteFileFromTable();

    void setCheckBox( int nState );
    void downloadFile();


    void selectAll();
    void selectNone();
    void downloadAll();


private:
    Ui::WidgetSystemScan *ui;
    ObjSystemScan* m_pObjSysScan;

    QThread* m_pThread;

//    QLabel*         m_pLabIco;
//    QLabel*         m_pLabTitlePro;
//    QLabel*         m_pLabTitleLst;
//    QLabel*         m_pLabWatch;
//    QPushButton*    m_pBtnScan;
//    QProgressBar*   m_pProBar;
//    QTableWidget*   m_pTableWidget;
//    QPushButton*    m_pBtnSelAll;
//    QPushButton*    m_pBtnSelNone;
//    QPushButton*    m_pBtnDownloadAll;

};

#endif // WIDGETSYSTEMSCAN_H
