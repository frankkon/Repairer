#-------------------------------------------------
#
# Project created by QtCreator 2014-08-17T13:41:44
#
#-------------------------------------------------

QT       += core gui
QT       += network

LIBS += -lshell32
LIBS += -lversion

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
    cleanitem.cpp

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
    cleanitem.h

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
