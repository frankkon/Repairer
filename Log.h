#pragma once

#include <string>
#include <fstream>

enum LOGLEVEL
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
};

class CLog
{
private:
    std::ofstream logFile;
    int m_iLogLevel;
    static CLog* instance;

public:
    CLog(void);
    explicit CLog(const char* strFilename);
    ~CLog(void);

    void setLogLevel(int iLogLevel);
    void openLogFile(const char* strFilename);
    void closeLogFile();
    void logDebug(const char* strLog);
    void logInfo(const char* strLog);
    void logWarn(const char* strLog);
    void logError(const char* strLog);

    static CLog* getInstance();
};






