#include "Log.h"

#include <ctime>
#include <iostream>
#include <cstdio> 
#include <string>

#define MAX_BUF_SIZE          2048
#define NORMAL_BUF_SIZE       128

std::string getLocalNowTime()
{
    time_t nowtime = time(0);
    struct tm _tm;
    localtime_s(&_tm, &nowtime);
    char szBuf[NORMAL_BUF_SIZE+1] = {0}; 
    strftime(szBuf, NORMAL_BUF_SIZE, "[%Y-%m-%d %X]", &_tm); 
    return szBuf;
}

CLog* CLog::instance = NULL;

CLog* CLog::getInstance()
{
    if(NULL != CLog::instance)
    {
        return CLog::instance;
    }

    CLog::instance = new CLog();
    if(NULL != CLog::instance)
    {
        CLog::instance->openLogFile("repairer-log.txt");
        //CLog::instance->setLogLevel(LOG_DEBUG);
    }

    return CLog::instance;
}

CLog::CLog(void):m_iLogLevel(LOG_INFO)
{
}

CLog::CLog(const char* strFilename):logFile(strFilename)
{
}

CLog::~CLog(void)
{
    //ofstream的析构函数会自动关闭并释放文件
    //logFile.close();
}

void CLog::setLogLevel(int iLogLevel)
{
    m_iLogLevel = iLogLevel;
}

void CLog::openLogFile(const char* strFilename)
{
    logFile.open(strFilename);
}

void CLog::closeLogFile()
{
	logFile.close();
}

void CLog::logDebug(const char* strLog)
{
    if(logFile && LOG_DEBUG <= m_iLogLevel)
    {
        logFile<<"[DEBUG]"<<getLocalNowTime()<<strLog<<std::endl;
    }
}

void CLog::logInfo(const char* strLog)
{
    if(logFile && LOG_INFO <= m_iLogLevel)
    {
       logFile<<"[INFO]"<<getLocalNowTime()<<strLog<<std::endl;
    }
}

void CLog::logWarn(const char* strLog)
{
    if(logFile && LOG_WARN <= m_iLogLevel)
    {
        logFile<<"[WARN]"<<getLocalNowTime()<<strLog<<std::endl;
    }
}

void CLog::logError(const char* strLog)
{
    if(logFile && LOG_ERROR <= m_iLogLevel)
    {
        logFile<<"[ERROR]"<<getLocalNowTime()<<strLog<<std::endl;
    }
}






