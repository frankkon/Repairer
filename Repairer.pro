#-------------------------------------------------
#
# Project created by QtCreator 2014-08-17T13:41:44
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql

LIBS += -lshell32
LIBS += -lversion
LIBS += -ladvapi32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dllescort
TEMPLATE = app

RC_FILE = Repair.rc
RC_FILE = resource.rc

#QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator'uiAccess='false'"
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"  #以管理员运行
#QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS,\"5.01\" #VS2013 在XP运行

SOURCES += main.cpp\
    widgetfilerepair.cpp \
    widgettop.cpp \
    widgetleft.cpp \
    objfilerepair.cpp \
    widgetmain.cpp \
    tool.cpp \
    global.cpp \
    widgetsystemscan.cpp \
    objsystemscan.cpp \
    dialogdrop.cpp \
    widgethome.cpp \
    http.cpp \
    filelistitem.cpp \
    filelistdownload.cpp \
    versionlistitem.cpp \
    widgethomesysinfo.cpp \
    dlgregisterkey.cpp \
    widgetregister.cpp \
    widgetversion.cpp \
    systemtray.cpp \
    widgetoption.cpp \
    yxyDES2.cpp \
    desmgr.cpp \
    widgetunregister.cpp \
    widgetdiskclean.cpp \
    objdiskclean.cpp \
    cleanitem.cpp \
    cdiskscanconfig.cpp \
    cwin10x86diskscanconfig.cpp \
    cwin10x64diskscanconfig.cpp \
    cwinxpx86diskscanconfig.cpp \
    cwinxpx64diskscanconfig.cpp \
    cwinvistax64diskscanconfig.cpp \
    cwinvistax86diskscanconfig.cpp \
    cwin7x86diskscanconfig.cpp \
    cwin7x64diskscanconfig.cpp \
    cwin8x64diskscanconfig.cpp \
    cwin8x86diskscanconfig.cpp \
    cwin81x64diskscanconfig.cpp \
    cwin81x86diskscanconfig.cpp \
    Log.cpp

HEADERS  += \
    widgetfilerepair.h \
    widgettop.h \
    widgetleft.h \
    objfilerepair.h \
    widgetmain.h \
    tool.h \
    global.h \
    widgetsystemscan.h \
    objsystemscan.h \
    dialogdrop.h \
    widgethome.h \
    http.h \
    filelistitem.h \
    filelistdownload.h \
    versionlistitem.h \
    widgethomesysinfo.h \
    dlgregisterkey.h \
    widgetregister.h \
    widgetversion.h \
    systemtray.h \
    widgetoption.h \
    yxyDES2.h \
    desmgr.h \
    widgetunregister.h \
    widgetdiskclean.h \
    objdiskclean.h \
    cleanitem.h \
    cdiskscanconfig.h \
    cwin10x86diskscanconfig.h \
    cwin10x64diskscanconfig.h \
    cwinxpx86diskscanconfig.h \
    cwinxpx64diskscanconfig.h \
    cwinvistax64diskscanconfig.h \
    cwinvistax86diskscanconfig.h \
    cwin7x86diskscanconfig.h \
    cwin7x64diskscanconfig.h \
    cwin8x64diskscanconfig.h \
    cwin8x86diskscanconfig.h \
    cwin81x64diskscanconfig.h \
    cwin81x86diskscanconfig.h \
    Log.h

RESOURCES += \
    Res.qrc

FORMS += \
    widgetversion.ui \
    widgetfilerepair.ui \
    widgetsystemscan.ui \
    WidgetHomeSysInfo.ui \
    widgetoption.ui \
    widgetunregister.ui \
    widgetdiskclean.ui
