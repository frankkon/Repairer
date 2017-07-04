#include "global.h"
#include "tool.h"
#include "windows.h"
#include "desmgr.h"
#include <QDebug>
#include <QDate>
#include <string>
#include <QSettings>
#include <QFile>

using namespace std;

QString Global::s_strVersion = "2014";
QString Global::s_strDatFile = "";
//QString Global::s_strHttpUrl = "http://file.dllescort.com";
//QString Global::s_strSystemUrl = "http://file.dllescort.com/win7_x64_u/";
//QString Global::s_strHttpUrl = "http://61.147.103.19:8089";
//QString Global::s_strSystemUrl = "http://61.147.103.19:8089";
QString Global::s_strHttpUrl = "http://file.dllescort.com";
QString Global::s_strSystemUrl = "http://file.dllescort.com";
QString Global::s_strDownloadPath;
QString Global::s_strSystemDrive;
bool Global::s_bActive = true;
QColor Global::s_clrWidgetTop = QColor(96,107,136);
QColor Global::s_clrWidgetLeft = QColor(71,84,99);
SeriesNumber Global::s_seriesNumber;
QString Global::s_strExeName = "dllescort";
QString Global::s_strExePath;
bool Global::s_bX64;
QString Global::s_strCurrentPath;
QString Global::s_strProgramDataPath;
QString Global::s_strTotalScan = "";



BOOL isSys32Bit()
{
    SYSTEM_INFO Si;
    ::GetNativeSystemInfo(&Si);
    if (PROCESSOR_ARCHITECTURE_AMD64 == Si.wProcessorArchitecture)
    {
        Global::s_bX64 = true;
        return FALSE;
    }
    else
    {
        Global::s_bX64 = false;
        return TRUE;
    }
}

BOOL isWindowsVersion( DWORD dwMajor, DWORD dwMinor )
{
    OSVERSIONINFOEX VersionInfo;
    DWORD dwTypeMask;
    DWORDLONG dwlConditionMask = 0;

    ZeroMemory(&VersionInfo, sizeof(OSVERSIONINFOEX));

    VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    VersionInfo.dwMajorVersion = dwMajor;
    VersionInfo.dwMinorVersion = dwMinor;
    dwTypeMask = VER_MAJORVERSION | VER_MINORVERSION;

    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

    return VerifyVersionInfo(&VersionInfo, dwTypeMask, dwlConditionMask);
}

QString getSysServicePack( OSVERSIONINFOEX& osInfo )
{
    QString strInfo;
    if (osInfo.wServicePackMajor > 0)
    {
        strInfo = QString(" Service Pack %1").arg(osInfo.wServicePackMajor);
    }

    return strInfo;
}

//#define PRODUCT_ULTIMATE                            0x00000001
//#define PRODUCT_HOME_BASIC                          0x00000002
//#define PRODUCT_HOME_PREMIUM                        0x00000003
//#define PRODUCT_ENTERPRISE                          0x00000004
//#define PRODUCT_HOME_BASIC_N                        0x00000005
//#define PRODUCT_BUSINESS                            0x00000006
//#define PRODUCT_STANDARD_SERVER                     0x00000007
//#define PRODUCT_DATACENTER_SERVER                   0x00000008
//#define PRODUCT_SMALLBUSINESS_SERVER                0x00000009
//#define PRODUCT_ENTERPRISE_SERVER                   0x0000000A
//#define PRODUCT_STARTER                             0x0000000B
//#define PRODUCT_DATACENTER_SERVER_CORE              0x0000000C
//#define PRODUCT_STANDARD_SERVER_CORE                0x0000000D
//#define PRODUCT_ENTERPRISE_SERVER_CORE              0x0000000E
//#define PRODUCT_ENTERPRISE_SERVER_IA64              0x0000000F
//#define PRODUCT_BUSINESS_N                          0x00000010
//#define PRODUCT_WEB_SERVER                          0x00000011
//#define PRODUCT_CLUSTER_SERVER                      0x00000012
//#define PRODUCT_HOME_SERVER                         0x00000013
//#define PRODUCT_STORAGE_EXPRESS_SERVER              0x00000014
//#define PRODUCT_STORAGE_STANDARD_SERVER             0x00000015
//#define PRODUCT_STORAGE_WORKGROUP_SERVER            0x00000016
//#define PRODUCT_STORAGE_ENTERPRISE_SERVER           0x00000017
//#define PRODUCT_SERVER_FOR_SMALLBUSINESS            0x00000018
//#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM        0x00000019
//#define PRODUCT_HOME_PREMIUM_N                      0x0000001A
//#define PRODUCT_ENTERPRISE_N                        0x0000001B
//#define PRODUCT_ULTIMATE_N                          0x0000001C
//#define PRODUCT_WEB_SERVER_CORE                     0x0000001D
//#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT    0x0000001E
//#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY      0x0000001F
//#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING     0x00000020
//#define PRODUCT_SERVER_FOUNDATION                   0x00000021
//#define PRODUCT_HOME_PREMIUM_SERVER                 0x00000022
//#define PRODUCT_SERVER_FOR_SMALLBUSINESS_V          0x00000023
//#define PRODUCT_STANDARD_SERVER_V                   0x00000024
//#define PRODUCT_DATACENTER_SERVER_V                 0x00000025
//#define PRODUCT_ENTERPRISE_SERVER_V                 0x00000026
//#define PRODUCT_DATACENTER_SERVER_CORE_V            0x00000027
//#define PRODUCT_STANDARD_SERVER_CORE_V              0x00000028
//#define PRODUCT_ENTERPRISE_SERVER_CORE_V            0x00000029
//#define PRODUCT_HYPERV                              0x0000002A
//#define PRODUCT_STORAGE_EXPRESS_SERVER_CORE         0x0000002B
//#define PRODUCT_STORAGE_STANDARD_SERVER_CORE        0x0000002C
//#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE       0x0000002D
//#define PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE      0x0000002E
//#define PRODUCT_STARTER_N                           0x0000002F
//#define PRODUCT_PROFESSIONAL                        0x00000030
//#define PRODUCT_PROFESSIONAL_N                      0x00000031
//#define PRODUCT_SB_SOLUTION_SERVER                  0x00000032
//#define PRODUCT_SERVER_FOR_SB_SOLUTIONS             0x00000033
//#define PRODUCT_STANDARD_SERVER_SOLUTIONS           0x00000034
//#define PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE      0x00000035
//#define PRODUCT_SB_SOLUTION_SERVER_EM               0x00000036
//#define PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM          0x00000037
//#define PRODUCT_SOLUTION_EMBEDDEDSERVER             0x00000038
//#define PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE        0x00000039
//#define PRODUCT_PROFESSIONAL_EMBEDDED               0x0000003A
//#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT       0x0000003B
//#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL       0x0000003C
//#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC    0x0000003D
//#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC    0x0000003E
//#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE   0x0000003F
//#define PRODUCT_CLUSTER_SERVER_V                    0x00000040
//#define PRODUCT_EMBEDDED                            0x00000041
//#define PRODUCT_STARTER_E                           0x00000042
//#define PRODUCT_HOME_BASIC_E                        0x00000043
//#define PRODUCT_HOME_PREMIUM_E                      0x00000044
//#define PRODUCT_PROFESSIONAL_E                      0x00000045
//#define PRODUCT_ENTERPRISE_E                        0x00000046
//#define PRODUCT_ULTIMATE_E                          0x00000047
//#define PRODUCT_ENTERPRISE_EVALUATION               0x00000048
//#define PRODUCT_MULTIPOINT_STANDARD_SERVER          0x0000004C
//#define PRODUCT_MULTIPOINT_PREMIUM_SERVER           0x0000004D
//#define PRODUCT_STANDARD_EVALUATION_SERVER          0x0000004F
//#define PRODUCT_DATACENTER_EVALUATION_SERVER        0x00000050
//#define PRODUCT_ENTERPRISE_N_EVALUATION             0x00000054
//#define PRODUCT_EMBEDDED_AUTOMOTIVE                 0x00000055
//#define PRODUCT_EMBEDDED_INDUSTRY_A                 0x00000056
//#define PRODUCT_THINPC                              0x00000057
//#define PRODUCT_EMBEDDED_A                          0x00000058
//#define PRODUCT_EMBEDDED_INDUSTRY                   0x00000059
//#define PRODUCT_EMBEDDED_E                          0x0000005A
//#define PRODUCT_EMBEDDED_INDUSTRY_E                 0x0000005B
//#define PRODUCT_EMBEDDED_INDUSTRY_A_E               0x0000005C
//#define PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER 0x0000005F
//#define PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER  0x00000060
//#define PRODUCT_CORE_ARM                            0x00000061
//#define PRODUCT_CORE_N                              0x00000062
//#define PRODUCT_CORE_COUNTRYSPECIFIC                0x00000063
//#define PRODUCT_CORE_SINGLELANGUAGE                 0x00000064
//#define PRODUCT_CORE                                0x00000065
//#define PRODUCT_PROFESSIONAL_WMC                    0x00000067
//#define PRODUCT_MOBILE_CORE                         0x00000068
#define PRODUCT_EMBEDDED_INDUSTRY_EVAL              0x00000069
#define PRODUCT_EMBEDDED_INDUSTRY_E_EVAL            0x0000006A
#define PRODUCT_EMBEDDED_EVAL                       0x0000006B
#define PRODUCT_EMBEDDED_E_EVAL                     0x0000006C
#define PRODUCT_CORE_SERVER                         0x0000006D
#define PRODUCT_CLOUD_STORAGE_SERVER                0x0000006E
#define PRODUCT_CORE_CONNECTED                      0x0000006F
#define PRODUCT_PROFESSIONAL_STUDENT                0x00000070
#define PRODUCT_CORE_CONNECTED_N                    0x00000071
#define PRODUCT_PROFESSIONAL_STUDENT_N              0x00000072
#define PRODUCT_CORE_CONNECTED_SINGLELANGUAGE       0x00000073
#define PRODUCT_CORE_CONNECTED_COUNTRYSPECIFIC      0x00000074

// typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);
// #define BUFSIZE 256

void Global::initGlobalData()
{


    //初始化下载文件存放目录
    Global::s_strDownloadPath = QString("%1\\FileDownload").arg(Global::s_strCurrentPath);
    Global::s_strDownloadPath.replace( "/", "\\" );


    Global::s_strExePath = QString("%1\\dllescort.exe").arg(Global::s_strCurrentPath);
    Global::s_strExePath.replace( "/", "\\" );
}

QString Global::getSysInfo()
{
    TCHAR sysDir[128];
    for( int i = 0; i < 128; i++ )
    {
        sysDir[i] = 0;
    }

    GetSystemDirectory( sysDir, 128 * sizeof(TCHAR));
    QString strSysDir = QString::fromWCharArray(sysDir);
    Global::s_strSystemDrive = strSysDir.left(3);

    PGPI pGPI;

    OSVERSIONINFOEX osInfo;
    string strWindowsVersion;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    ::GetVersionEx((LPOSVERSIONINFO)&osInfo);

    QString strInfo;

    //E_SYS_VERSION eSysVersion = E_SYS_VERSION_NOT_SUPPORTED;
    QString strFile = "os\\";


// 	OSVERSIONINFOEX osvi;
// 	SYSTEM_INFO si;
// 	PGNSI pGNSI;
// 	PGPI pGPI;
// 	BOOL bOsVersionInfoEx;
// 	DWORD dwType;
// 	ZeroMemory(&si, sizeof(SYSTEM_INFO));
// 	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
// 	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
// 	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);
// 	if(!bOsVersionInfoEx)
// 		return FALSE;   // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.
// 	pGNSI = (PGNSI) GetProcAddress(      GetModuleHandle(TEXT("kernel32.dll")),       "GetNativeSystemInfo");
//
// 	if(NULL != pGNSI)
// 		pGNSI(&si);
// 	else
// 		GetSystemInfo(&si);





    //pGPI = (PGPI) GetProcAddress(            GetModuleHandle(TEXT("kernel32.dll")),             "GetProductInfo");

    //pGPI( osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.wServicePackMajor, osInfo.wServicePackMinor, (PDWORD)&dwProductType );
    DWORD dwProductType = 0;
    pGPI = (PGPI)GetProcAddress( GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");

    switch (osInfo.dwMajorVersion)
    {
    case 6:

        //::GetProductInfo(osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.wServicePackMajor, osInfo.wServicePackMinor, (PDWORD)&dwProductType);
        pGPI( osInfo.dwMajorVersion, osInfo.dwMinorVersion, osInfo.wServicePackMajor, osInfo.wServicePackMinor, (PDWORD)&dwProductType );

        switch (osInfo.dwMinorVersion)
        {
        case 0:
            if (VER_NT_WORKSTATION == osInfo.wProductType)  //Windows Vista
            {
                strInfo = "Windows Vista";
                strFile += "vista\\";

//                wchar_t w[1024] = { 0 };
//                GetEnvironmentVariable( L"ProgramData", w, 1024 );
//                Global::s_strProgramDataPath = QString::fromWCharArray(w);
                Global::s_strProgramDataPath = Global::s_strSystemDrive + "ProgramData";

                if (isSys32Bit())
                {
                    //eSysVersion = E_SYS_VERSION_VISTA_X86;

                    if (PRODUCT_BUSINESS == dwProductType || PRODUCT_BUSINESS_N == dwProductType)
                    {
                        strFile += "vista_x86_b.txt";
                        strInfo += " Business";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86_b/";
                    }
                    //else if (PRODUCT_ENTERPRISE == dwProductType || PRODUCT_ENTERPRISE_N == dwProductType)
                    else if (PRODUCT_ENTERPRISE == dwProductType || 0x0000001B == dwProductType)
                    {
                        strFile += "vista_x86_e.txt";
                        strInfo += " Enterprise";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86_e/";
                    }
                    else if (PRODUCT_HOME_BASIC == dwProductType || PRODUCT_HOME_BASIC_N == dwProductType)
                    {
                        strFile += "vista_x86_hb.txt";
                        strInfo += " Home Basic";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86_hb/";
                    }
                    //else if (PRODUCT_HOME_PREMIUM == dwProductType || PRODUCT_HOME_PREMIUM_N == dwProductType)
                    else if (PRODUCT_HOME_PREMIUM == dwProductType || 0x0000001A == dwProductType)
                    {
                        strFile += "vista_x86_hp.txt";
                        strInfo += " Home Premium";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86_hp/";
                    }
                    //else if (PRODUCT_ULTIMATE == dwProductType || PRODUCT_ULTIMATE_N == dwProductType)
                    else if (PRODUCT_ULTIMATE == dwProductType || PRODUCT_ULTIMATE_N == dwProductType)
                    {
                        strFile += "vista_x86_u.txt";
                        strInfo += " Ultimate";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86_u/";
                    }
                    else
                    {
                        strFile += "vista_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86/";
                    }
                }
                else
                {
                    //eSysVersion = E_SYS_VERSION_VISTA_X64;

                    if (PRODUCT_BUSINESS == dwProductType || PRODUCT_BUSINESS_N == dwProductType)
                    {
                        strFile += "vista_x64_b.txt";
                        strInfo += " Business";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x64_b/";
                    }
                    else if (PRODUCT_HOME_BASIC == dwProductType || PRODUCT_HOME_BASIC_N == dwProductType)
                    {
                        strFile += "vista_x64_hb.txt";
                        strInfo += " Home Basic";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x64_hb/";
                    }
                    else if (PRODUCT_HOME_PREMIUM == dwProductType || PRODUCT_HOME_PREMIUM_N == dwProductType)
                    {
                        strFile += "vista_x64_hp.txt";
                        strInfo += " Home Premium";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x64_hp/";
                    }
                    else if (PRODUCT_ULTIMATE == dwProductType || PRODUCT_ULTIMATE_N == dwProductType)
                    {
                        strFile += "vista_x86_u.txt";
                        strInfo += " Ultimate";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x86_u/";
                    }
                    else
                    {
                        strFile += "vista_x64_hb.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/vista_x64_hb/";
                    }
                } 
            }
            else  //Windows Server 2008
            {
                strInfo = "Windows Server 2008";
            }

            break;
        case 1:
            if (VER_NT_WORKSTATION == osInfo.wProductType)  //Windows 7
            {
                strInfo = "Windows 7";
                strFile += "win7\\";

//                wchar_t w[1024] = { 0 };
//                GetEnvironmentVariable( L"ProgramData", w, 1024 );
//                Global::s_strProgramDataPath = QString::fromWCharArray(w);
                Global::s_strProgramDataPath = Global::s_strSystemDrive + "ProgramData";

                if (isSys32Bit())
                {
                    //eSysVersion = E_SYS_VERSION_WIN7_X86;

                    if (PRODUCT_HOME_BASIC == dwProductType || PRODUCT_HOME_BASIC_N == dwProductType)
                    {
                        strFile += "win7_x86_home.txt";
                        strInfo += " Home Basic";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x86_home/";
                    }
                    else if (PRODUCT_HOME_PREMIUM == dwProductType || PRODUCT_HOME_PREMIUM_N == dwProductType)
                    {
                        strFile += "win7_x86_home.txt";
                        strInfo += " Home Premium";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x86_home/";
                    }
                    else if (PRODUCT_PROFESSIONAL == dwProductType || PRODUCT_PROFESSIONAL_N == dwProductType)
                    {
                        strFile += "win7_x86_pro.txt";
                        strInfo += " Professional";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x86_pro/";
                    }
                    else if (PRODUCT_ULTIMATE == dwProductType || PRODUCT_ULTIMATE_N == dwProductType)
                    {
                        strFile += "win7_x86_u.txt";
                        strInfo += " Ultimate";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x86_u/";
                    }
                    else
                    {
                        strFile += "win7_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x86/";
                    }
                }
                else
                {
                    //eSysVersion = E_SYS_VERSION_WIN7_X64;

                    if (PRODUCT_HOME_BASIC == dwProductType || PRODUCT_HOME_BASIC_N == dwProductType)
                    {
                        strFile += "win7_x64_home.txt";
                        strInfo += " Home Basic";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x64_home/";
                    }
                    else if (PRODUCT_HOME_PREMIUM == dwProductType || PRODUCT_HOME_PREMIUM_N == dwProductType)
                    {
                        strFile += "win7_x64_home.txt";
                        strInfo += " Home Premium";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x64_home/";
                    }
                    else if (PRODUCT_PROFESSIONAL == dwProductType || PRODUCT_PROFESSIONAL_N == dwProductType)
                    {
                        strFile += "win7_x64_pro.txt";
                        strInfo += " Professional";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x64_pro/";
                    }
                    else if (PRODUCT_ULTIMATE == dwProductType || PRODUCT_ULTIMATE_N == dwProductType)
                    {
                        strFile += "win7_x64_u.txt";
                        strInfo += " Ultimate";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x64_u/";
                    }
                    else
                    {
                        strFile += "win7_x64_home.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/win7_x64_home/";
                    }
                }
            }
            else  //Windows Server 2008 R2
            {
                strInfo = "Windows Server 2008 R2";
            }

            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
        }
        break;
    case 5:
        switch (osInfo.dwMinorVersion)
        {
        case 1:
            strInfo = "Windows XP";
            strFile += "winxp\\";

//            wchar_t w[1024] = { 0 };
//            GetEnvironmentVariable( L"ALLUSERSPROFILE", w, 1024 );
//            Global::s_strProgramDataPath = QString::fromWCharArray(w);
//            Global::s_strProgramDataPath += "\\Application Data";
            Global::s_strProgramDataPath = Global::s_strSystemDrive + "Documents and Settings\\All Users\\Application Data";

            if (isSys32Bit())
            {
                //测试
//                {
//                QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件
//                settings.beginGroup("Option");
//                QString s = QString("%1").arg(osInfo.wSuiteMask);
//                settings.setValue("osInfo.wSuiteMask", s );
//                settings.endGroup();
//                }

                //if (osInfo.wSuiteMask && osInfo.wSuiteMask == VER_SUITE_PERSONAL)  //osvi.wSuiteMask & VER_SUITE_PERSONAL
                if (osInfo.wSuiteMask & VER_SUITE_PERSONAL)  //osvi.wSuiteMask & VER_SUITE_PERSONAL
                {
                    strInfo += " Home Edition";

                    if (2 == osInfo.wServicePackMajor)
                    {
                        strFile += "xpsp2_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/xpsp2_x86/";
                    }
                    else if (3 == osInfo.wServicePackMajor)
                    {
                        strFile += "xpsp3_x86_home.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/xpsp3_x86_home/";
                    }
                    else
                    {
                        strFile += "xpsp2_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/xpsp2_x86/";
                    }
                }
                else
                {
                    strInfo += " Professional";

                    if (2 == osInfo.wServicePackMajor)
                    {
                        strFile += "xpsp2_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/xpsp2_x86/";
                    }
                    else if (3 == osInfo.wServicePackMajor)
                    {
                        strFile += "xpsp3_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/xpsp3_x86/";
                    }
                    else
                    {
                        strFile += "xpsp2_x86.txt";
                        Global::s_strSystemUrl = Global::s_strHttpUrl + "/xpsp2_x86/";
                    }
                }

            }
            else
            {
                if (osInfo.wSuiteMask & VER_SUITE_PERSONAL)
                {
                    strInfo += " Home Edition";
                }
                else
                {
                    strInfo += " Professional";
                }

                strFile += "xp_x64.txt";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/xp_x64/";
            }
        }
        break;
    default:
        break;
    }

    //拷贝kernel32.dll到exe目录下，否则获取的版本信息不正确
    if(!QFile::copy( strSysDir+"\\kernel32.dll", "os.dll" ))
    {
        QString strTmp = QString("copy %1 to %2 failed").arg(strSysDir+"\\kernel32.dll").arg("os.dll");
        qDebug()<<strTmp;
    }
    QString strKernel32Version = Tool::getFileVersion("os.dll");
    int nKernelVersion = strKernel32Version.left(strKernel32Version.indexOf(".")).toInt();

    bool bGetVersion = false;

    //判断win10
    if(nKernelVersion == 10)
    {
        bGetVersion = true;

        strInfo = "Windows 10";
        strFile += "win10\\";

        Global::s_strProgramDataPath = Global::s_strSystemDrive + "ProgramData";

        if (isSys32Bit())
        {
            if( dwProductType == 48 )
            {
                strFile += "win10_x86_pro.txt";
                strInfo += " Professional";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/win10_x86_pro/";
            }
            else if(dwProductType == 4)
            {
                strFile += "win10_x86_e.txt";
                strInfo += " Enterprise";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/win10_x86_e/";
            }
            else  //家庭版101
            {
                strFile += "win10_x86_h.txt";
                strInfo += " Home";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/win10_x86_e/";
            }
        }
        else
        {
            if( dwProductType == 48 )
            {
                strFile += "win10_x64_pro.txt";
                strInfo += " Professional";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/win10_x64_pro/";
            }
            else if(dwProductType == 4)
            {
                strFile += "win10_x64_e.txt";
                strInfo += " Enterprise";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/win10_x64_e/";
            }
            else  //家庭版101
            {
                strFile += "win10_x64_h.txt";
                strInfo += " Home";
                Global::s_strSystemUrl = Global::s_strHttpUrl + "/win10_x64_e/";
            }
        }
    }


    if(!bGetVersion and isWindowsVersion( 6,2 ))
    {
        if (VER_NT_WORKSTATION == osInfo.wProductType)  //Windows 8
        {
            strInfo = "Windows 8";
            strFile += "win8\\";

//            wchar_t w[1024] = { 0 };
//            GetEnvironmentVariable( L"ProgramData", w, 1024 );
//            Global::s_strProgramDataPath = QString::fromWCharArray(w);
            Global::s_strProgramDataPath = Global::s_strSystemDrive + "ProgramData";

            if (isSys32Bit())
            {
                //eSysVersion = E_SYS_VERSION_WIN8_X86;

                if (PRODUCT_ENTERPRISE == dwProductType || PRODUCT_ENTERPRISE_N == dwProductType)
                {
                    strFile += "win8_x86_e.txt";
                    strInfo += " Enterprise";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8_x86_e/";
                }
                else if (PRODUCT_PROFESSIONAL == dwProductType || PRODUCT_PROFESSIONAL == dwProductType)
                {
                    strFile += "win8_x86_pro.txt";
                    strInfo += " Professional";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8_x86_pro/";
                }
                else
                {
                    strFile += "win8_x86.txt";
                }
            }
            else
            {
                //eSysVersion = E_SYS_VERSION_WIN8_X64;

                if (PRODUCT_ENTERPRISE == dwProductType || PRODUCT_ENTERPRISE_N == dwProductType)
                {
                    strFile += "win8_x64_e.txt";
                    strInfo += " Enterprise";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8_x64_e/";
                }
                else if (PRODUCT_PROFESSIONAL == dwProductType || PRODUCT_PROFESSIONAL == dwProductType)
                {
                    strFile += "win8_x64_pro.txt";
                    strInfo += " Professional";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8_x64_pro/";
                }
                else
                {
                    strFile += "win8_x64.txt";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8_x64/";
                }
            }
        }
        else  //Windows Server 2012
        {
            strInfo = "Windows Server 2012";
        }
    }
    else if(isWindowsVersion( 6,3 ))
    {
        if (VER_NT_WORKSTATION == osInfo.wProductType)  //Windows 8.1
        {
            strInfo = "Windows 8.1";
            strFile += "win8.1\\";

//            wchar_t w[1024] = { 0 };
//            GetEnvironmentVariable( L"ProgramData", w, 1024 );
//            Global::s_strProgramDataPath = QString::fromWCharArray(w);
            Global::s_strProgramDataPath = Global::s_strSystemDrive + "ProgramData";

            if (isSys32Bit())
            {
                //eSysVersion = E_SYS_VERSION_WIN8_1_X86;

                if (PRODUCT_ENTERPRISE == dwProductType || PRODUCT_ENTERPRISE_N == dwProductType)
                {
                    strFile += "win8.1_x86_e.txt";
                    strInfo += " Enterprise";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8.1_x86_e/";
                }
                else if (PRODUCT_PROFESSIONAL == dwProductType || PRODUCT_PROFESSIONAL == dwProductType)
                {
                    strFile += "win8.1_x86_pro.txt";
                    strInfo += " Professional";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8.1_x86_pro/";
                }
                else
                {
                    strFile += "win8.1_x86_e.txt";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8.1_x86_e/";
                }
            }
            else
            {
                //eSysVersion = E_SYS_VERSION_WIN8_1_X64;

                if (PRODUCT_ENTERPRISE == dwProductType || PRODUCT_ENTERPRISE_N == dwProductType)
                {
                    strFile += "win8.1_x64_e.txt";
                    strInfo += " Enterprise";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8.1_x64_e/";
                }
                else if (PRODUCT_PROFESSIONAL == dwProductType || PRODUCT_PROFESSIONAL == dwProductType)
                {
                    strFile += "win8.1_x64_pro.txt";
                    strInfo += " Professional";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8.1_x64_pro/";
                }
                else
                {
                    strFile += "win8.1_x64_e.txt";
                    Global::s_strSystemUrl = Global::s_strHttpUrl + "/win8.1_x64_e/";
                }
            }
        }
        else  //Windows Server 2012 R2
        {
            strInfo = "Windows Server 2012 R2";
        }

    }

    if (osInfo.wServicePackMajor > 0)
    {
        strInfo += getSysServicePack(osInfo);
    }


    if (isSys32Bit())
    {
        strInfo += " 32Bit";
    }
    else
    {
        strInfo += " 64Bit";
    }

    Global::s_strDatFile = strFile;
    //测试
//    {
//    QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件
//    settings.beginGroup("Option");
//    settings.setValue("Global::s_strDatFile", Global::s_strDatFile );
//    settings.endGroup();
//    }



//    CString str(strFile.c_str());
//    CString strType;
//    strType.Format(_T("dwProductType:%d     "), dwProductType);
//    strType += str;
//    AfxMessageBox(strType);

    return  strInfo;
}

bool Global::isKeyUnregistered( QString strKey )
{
    QString strPath = QString("%1\\dllescort\\data.dat").arg(Global::s_strProgramDataPath);

    QFile file(strPath);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"open file "<<strPath<<" failed,error:"<<file.errorString();
    }
    else
    {

    }

    QTextStream txtInput(&file);

    bool bInvalid = false;
    while(!txtInput.atEnd())
    {
        QString strLine = txtInput.readLine();
        if( strLine == strKey )
        {
            bInvalid = true;
            break;
        }
    }
    file.close();

    return bInvalid;
}

bool Global::decodeSeriesNumber( QString strEncode )
{
    if(strEncode.size()!=48)
    {
        return false;
    }

    s_bActive = false;

    DesMgr d;
    QString strCode = d.decrypt(strEncode);



    if(isKeyUnregistered(strEncode))
    {
        s_bActive = false;
        return false;
    }



    QString strVersion = strCode.mid(0, 2);
    s_seriesNumber.nExpiredYear = strCode.mid(2, 4).toInt();
    s_seriesNumber.nExpiredMonth = strCode.mid(6, 2).toInt();
    s_seriesNumber.nExpiredDay = strCode.mid(8, 2).toInt();

    s_seriesNumber.nExeYear = strCode.mid(10, 4).toInt();
    s_seriesNumber.nExeMonth = strCode.mid(14, 2).toInt();
    s_seriesNumber.nExeDay = strCode.mid(16, 2).toInt();

    s_seriesNumber.strEndode = strEncode;
    s_seriesNumber.strDecode = strCode;

    QDate date = QDate::currentDate();
    int nYear = date.year();
    int nMonth = date.month();
    int nDay = date.day();

    if( strVersion != "01" )
    {
        s_bActive = false;
        return false;
    }

    if( 0 == s_seriesNumber.nExpiredYear || 0 == s_seriesNumber.nExeYear )
    {
        s_bActive = false;
        return false;
    }

    if( s_seriesNumber.nExpiredMonth < 1 || s_seriesNumber.nExpiredMonth > 12 )
    {
        s_bActive = false;
        return false;
    }
    if( s_seriesNumber.nExeMonth < 1 || s_seriesNumber.nExeMonth > 12 )
    {
        s_bActive = false;
        return false;
    }

    if( s_seriesNumber.nExpiredDay < 1 || s_seriesNumber.nExpiredDay > 31 )
    {
        s_bActive = false;
        return false;
    }
    if( s_seriesNumber.nExeDay < 1 || s_seriesNumber.nExeDay > 31 )
    {
        s_bActive = false;
        return false;
    }

    if( nYear < s_seriesNumber.nExpiredYear )
    {
        s_bActive = true;
    }
    else if( nYear == s_seriesNumber.nExpiredYear )
    {
        if( nMonth < s_seriesNumber.nExpiredMonth )
        {
            s_bActive = true;
        }
        else if( nMonth == s_seriesNumber.nExpiredMonth )
        {
            if( nDay <= s_seriesNumber.nExpiredDay )
            {
                s_bActive = true;
            }
        }
    }

    if( nYear < s_seriesNumber.nExeYear )
    {
        s_bActive = true;
    }
    else if( nYear == s_seriesNumber.nExeYear )
    {
        if( nMonth < s_seriesNumber.nExeMonth )
        {
            s_bActive = true;
        }
        else if( nMonth == s_seriesNumber.nExeMonth )
        {
            if( nDay <= s_seriesNumber.nExeDay )
            {
                s_bActive = true;
            }
        }
    }

    return s_bActive;
}

void Global::readIni()
{
    QSettings settings("option.ini", QSettings::IniFormat);

    QString strSeriesNumber = settings.value("Option/SeriesNumber").toString();

    if(strSeriesNumber.isEmpty())
    {
        QString strPath = QString("%1\\dllescort\\option.ini").arg(Global::s_strProgramDataPath);

        QSettings settings2(strPath, QSettings::IniFormat);
        QString strSeriesNumber2 = settings2.value("Option/SeriesNumber").toString();

        if(Global::decodeSeriesNumber(strSeriesNumber2))
        {
            settings.beginGroup("Option");
            settings.setValue("SeriesNumber", strSeriesNumber2 );
            settings.endGroup();
        }
    }
    else
    {
        Global::decodeSeriesNumber(strSeriesNumber);
    }
}

void Global::readCurrentPath()
{
    QSettings settings("option.ini", QSettings::IniFormat);
    Global::s_strCurrentPath = settings.value("Option/CurrentPath").toString();
}

void Global::saveCurrentPath()
{
    QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件

    settings.beginGroup("Option");
    settings.setValue("CurrentPath", Global::s_strCurrentPath );
    settings.endGroup();
}

void Global::saveFreeFixUnRegister( QString strFlag )
{
    QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("Option");
    settings.setValue("FreeFixUnRegister", strFlag );
    settings.endGroup();

    //备份的路径
    {
        QString strPath = QString("%1\\dllescort\\option.ini").arg(Global::s_strProgramDataPath);
        if(!Tool::isFileOrDirExist(strPath))
        {
            Tool::createDirectory(strPath,true);
        }

        QSettings settings(strPath, QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup("Option");
        settings.setValue("FreeFixUnRegister", strFlag );
        settings.endGroup();
    }
}

QString Global::readFreeFixUnRegister()
{
    QSettings settings("option.ini", QSettings::IniFormat);

    QString strFreeFixUnRegister = settings.value("Option/FreeFixUnRegister").toString();

    if(strFreeFixUnRegister.isEmpty())
    {
        QString strPath = QString("%1\\dllescort\\option.ini").arg(Global::s_strProgramDataPath);

        QSettings settings2(strPath, QSettings::IniFormat);
        QString strFreeFixUnRegister2 = settings2.value("Option/FreeFixUnRegister").toString();

        if(!strFreeFixUnRegister2.isEmpty())
        {
            settings.beginGroup("Option");
            settings.setValue("FreeFixUnRegister", strFreeFixUnRegister2 );
            settings.endGroup();
        }

        return strFreeFixUnRegister2;
    }

    return strFreeFixUnRegister;
}

void Global::saveIniData( QString strKey, QString strValue )
{
    QSettings settings("option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.beginGroup("Option");
    settings.setValue( strKey, strValue );
    settings.endGroup();

    //备份的路径
    {
        QString strPath = QString("%1\\dllescort\\option.ini").arg(Global::s_strProgramDataPath);
        if(!Tool::isFileOrDirExist(strPath))
        {
            Tool::createDirectory(strPath,true);
        }

        QSettings settings(strPath, QSettings::IniFormat); // 当前目录的INI文件
        settings.beginGroup("Option");
        settings.setValue( strKey, strValue );
        settings.endGroup();
    }
}

QString Global::readIniData( QString strKey )
{
    QSettings settings("option.ini", QSettings::IniFormat);

    QString strFullKey = "Option//" + strKey;
    QString strValue = settings.value(strFullKey).toString();

    if(strValue.isEmpty())
    {
        QString strPath = QString("%1\\dllescort\\option.ini").arg(Global::s_strProgramDataPath);

        QSettings settings2(strPath, QSettings::IniFormat);
        QString strValue2 = settings2.value(strFullKey).toString();

        if(!strValue2.isEmpty())
        {
            settings.beginGroup("Option");
            settings.setValue( strKey, strValue2 );
            settings.endGroup();
        }

        return strValue2;
    }

    return strValue;
}

