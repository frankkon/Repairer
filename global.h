#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QColor>

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

struct SeriesNumber
{
    QString strEndode;
    QString strDecode;
    int nExpiredYear;
    int nExpiredMonth;
    int nExpiredDay;

    int nExeYear;
    int nExeMonth;
    int nExeDay;


};

class Global
{
public:
    static void checkSystemVersion();
    static bool decodeSeriesNumber( QString strEncode );
    static QString getSysInfo();

    static void initGlobalData();


    static void readIni();
    static void readCurrentPath();
    static void saveCurrentPath();
    static void saveFreeFixUnRegister( QString strFlag );
    static QString readFreeFixUnRegister();

    static void saveIniData( QString strKey, QString strValue );
    static QString readIniData( QString strKey );


private:
    static bool isKeyUnregistered( QString strKey );



public:
    static QString s_strCurrentPath;
    static QString s_strProgramDataPath;
    static QString s_strSystemDrive;
    static QString s_strExePath;
    static QString s_strHttpUrl;
    static QString s_strDownloadPath;
    static QString s_strSystemUrl;

    static QString s_strVersion;
    static QString s_strDatFile;

    static QColor s_clrWidgetTop;
    static QColor s_clrWidgetLeft;

    static QString s_strTotalScan;

    static SeriesNumber s_seriesNumber;
    static bool s_bActive;

    static bool s_bX64;

    static QString s_strExeName;
};

#endif // GLOBAL_H
