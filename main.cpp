#include "widgetmain.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include "global.h"
#include "tool.h"

#include "desmgr.h"
#include <windows.h>
#include <QSharedMemory>

void test()
{
    QSharedMemory shared_memory;
    shared_memory.setKey("85524");
    if(shared_memory.attach())
    {
        return;
    }

    shared_memory.create(1);

    WidgetMain w;
    if(2 > 1)
    {
        w.hide();

        QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        QSettings reg(path, QSettings::NativeFormat);
        QString value = reg.value("dllescort").toString();
        value = value.left(value.lastIndexOf("\\dllescort.exe"));
        Global::s_strCurrentPath = value;
    }
    else
    {
        w.show();
        Global::s_strCurrentPath = QDir::currentPath();
    }

    qDebug()<<"main thread:"<<QThread::currentThreadId();

    //a.setWindowIcon(QIcon(":/main/inno"));

    //初始化
     Global::initGlobalData();

    w.init();

    Global::readIni();

    {
        QString str = "C:/Documents and Settings/Administrator/Cookies/administrator@114la[1].txt";
        qDebug()<<str;
        str.replace("/", "\\");
        qDebug()<<str;
    }
}

void test2()
{
    QSharedMemory shared_memory;
    shared_memory.setKey("85524");
    if(shared_memory.attach())
    {
        return;
    }

    shared_memory.create(1);

    WidgetMain w;
    if(2 > 1)
    {
        w.hide();

        QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        QSettings reg(path, QSettings::NativeFormat);
        QString value = reg.value("dllescort").toString();
        value = value.left(value.lastIndexOf("\\dllescort.exe"));
        Global::s_strCurrentPath = value;
    }
    else
    {
        w.show();
        Global::s_strCurrentPath = QDir::currentPath();
    }

    qDebug()<<"main thread:"<<QThread::currentThreadId();

    //a.setWindowIcon(QIcon(":/main/inno"));

    //初始化
     Global::initGlobalData();

    w.init();

    Global::readIni();

}

int main(int argc, char *argv[])
{
    //QString strVersion = Tool::getFileVersion("c:\\kernel32.dll");
    //int nf = strVersion.indexOf(".");
    //int vv = strVersion.left(nf).toInt();

//    qint64 qi = -2;
//    QString strText = "Error downloading http://61.147.103.19:8089/xpsp3_x86/AppPatch/test.dll - server replied: Not Found";
//    int nff = strText.indexOf("tserver replied: Not Found");

    //测试搜索磁盘
    /*
    UINT typeDev = 0;
    wchar_t ch[4] = { 'X', ':', '\\', '\0' };
    for( wchar_t c = 'A'; c <= 'Z'; c++ )
    {
        ch[0] = c;
        typeDev = ::GetDriveType(ch);
        if( typeDev == DRIVE_FIXED )
        {
            QString strDev = QString::fromWCharArray(ch);
            qDebug()<<"分区："<<strDev;
        }
    }
    */

     QApplication a(argc, argv);

    //限制只能启动一个进程
    QSharedMemory shared_memory;
    shared_memory.setKey("85524");
    if(shared_memory.attach())
    {
        return 0;
    }

    shared_memory.create(1);

    WidgetMain w;
    if(argc > 1)
    {
        w.hide();

        QString path = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
        QSettings reg(path, QSettings::NativeFormat);
        QString value = reg.value("dllescort").toString();
        value = value.left(value.lastIndexOf("\\dllescort.exe"));
        Global::s_strCurrentPath = value;
    }
    else
    {
        w.show();
        Global::s_strCurrentPath = QDir::currentPath();
    }

    qDebug()<<"main thread:"<<QThread::currentThreadId();

    a.setWindowIcon(QIcon(":/main/inno"));

    //初始化
     Global::initGlobalData();

    w.init();

    Global::readIni();

    return a.exec();
}
