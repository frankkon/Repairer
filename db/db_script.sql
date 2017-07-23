-- 数据库文件名称：repairer.db

create database repairer_db;

create table disk_scan_info
(
    id                     integer,           --主键，唯一标识
    platform_id            integer,           --windows版本ID
    platform_name          varchar(20),       --windows版本描述
    platform_cpu           integer,           --0:x86;1:x64
    desc_cn                varchar(256),      --垃圾文件类型中文描述
    desc_en                varchar(256),      --垃圾文件类型英文描述
    trash_path             varchar(1024),     --垃圾文件的绝对路径
    ext_name               varchar(512),      --带扫描路径下垃圾文件的扩展名列表，分号（;）隔开
    clean_all_flag         integer,           --待扫描的路径下的文件是否全部删除，0：根据ext_name操作，1：全部删除 
    PRIMARY KEY (id)    
);

create table reg_scan_info
(
    id                     integer,           --主键，唯一标识
    platform_id            integer,           --windows版本ID
    platform_name          varchar(20),       --windows版本描述
    platform_cpu           integer,           --0:x86;1:x64
    err_type               integer,           --错误类型，不同的类型不同的处理方式
    desc_cn                varchar(256),      --注册表错误类型中文描述
    desc_en                varchar(256),      --注册表错误类型英文描述
    reg_path               varchar(1024),     --注册表错误的绝对路径
    PRIMARY KEY (id)    
);

-- ///////////////////////////////////////////////////////////////////////////////////////////////
-- //disk_scan_info
-- ///////////////////////////////////////////////////////////////////////////////////////////////
-- windows xp x86
insert into disk_scan_info values(5100, 51, "Windows_xp_x86", 0, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\Local Settings\Temp","", 1);
-- insert into disk_scan_info values(5101, 51, "Windows_xp_x86", 0, "Windows 系统日志文件", "Windows System Log Files", "C:\Windows\Logs","", 1);
-- insert into disk_scan_info values(5102, 51, "Windows_xp_x86", 0, "IIS日志文件", "IIS Log Files", "C:\Windows\System32\LogFiles","", 1);
-- insert into disk_scan_info values(5105, 51, "Windows_xp_x86", 0, "Windows WinSxs备份缓存", "Windows WinSxs Backup Cache", "C:\Windows\WinSxS\Backup","", 1);
-- insert into disk_scan_info values(5106, 51, "Windows_xp_x86", 0, "manifast cashe缓存", "manifast cashe", "C:\Windows\WinSxS\ManifestCache","", 1);
-- insert into disk_scan_info values(5107, 51, "Windows_xp_x86", 0, "缩略图缓存", "Thumb Cache", "%UserProfile%\AppData\Local\Microsoft\Windows\Explorer","", 1);
insert into disk_scan_info values(5108, 51, "Windows_xp_x86", 0, "系统图标缓存", "System Icon Cache", "%UserProfile%\Local Settings\Application Data","IconCache.db", 0);
-- insert into disk_scan_info values(5109, 51, "Windows_xp_x86", 0, "无效快捷方式", "Invalid shortcut", "%UserProfile%\AppData\Roaming\Microsoft\Windows\Recent","", 1);
insert into disk_scan_info values(5110, 51, "Windows_xp_x86", 0, "IE浏览器缓存", "IE Cache", "%UserProfile%\Local Settings\Temporary Internet Files","", 1);

-- windows xp x64
insert into disk_scan_info values(5250, 52, "Windows_xp_x64", 1, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows vista x86
insert into disk_scan_info values(6000, 60, "Windows_vista_x86", 0, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows vista x64
insert into disk_scan_info values(6050, 60, "Windows_vista_x64", 1, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 7 x86
insert into disk_scan_info values(6100, 61, "Windows_7_x86", 0, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 7 x64
insert into disk_scan_info values(6150, 61, "Windows_7_x64", 1, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 8 x86
insert into disk_scan_info values(6200, 62, "Windows_8_x86", 0, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 8 x64
insert into disk_scan_info values(6250, 62, "Windows_8_x64", 1, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 8.1 x86
insert into disk_scan_info values(6300, 63, "Windows_81_x86", 0, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 8.1 x64
insert into disk_scan_info values(6350, 63, "Windows_81_x64", 1, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- windows 10 x86
insert into disk_scan_info values(10000, 100, "Windows_10_x86", 0, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);
insert into disk_scan_info values(10001, 100, "Windows_10_x86", 0, "Windows Installer临时文件", "Windows Installer Temp Files", "C:\Windows\Installer\$PatchCache$\Managed","", 1);

insert into disk_scan_info values(10003, 100, "Windows_10_x86", 0, "微软Defender扫描历史", "Microsoft Defender Scan History", "C:\ProgramData\Microsoft\Windows Defender\Support","*.log;*.bin", 0);
insert into disk_scan_info values(10004, 100, "Windows_10_x86", 0, "微软Defender扫描历史", "Microsoft Defender Scan History", "C:\ProgramData\Microsoft\Windows Defender\Scans","*.bin*", 0);
insert into disk_scan_info values(10005, 100, "Windows_10_x86", 0, "微软Defender扫描历史", "Microsoft Defender Scan History", "C:\ProgramData\Microsoft\Windows Defender\Scans\History\Results\Resource","", 1);
insert into disk_scan_info values(10006, 100, "Windows_10_x86", 0, "微软Defender扫描历史", "Microsoft Defender Scan History", "C:\Documents and Settings\All Users\Application Data\Microsoft\Windows Defender\Scans\History\CacheManager","*.bin", 0);
insert into disk_scan_info values(10007, 100, "Windows_10_x86", 0, "微软Defender扫描历史", "Microsoft Defender Scan History", "C:\ProgramData\Application Data\Microsoft\Windows Defender\Definition Updates\Backup","", 1);
insert into disk_scan_info values(10008, 100, "Windows_10_x86", 0, "微软Defender扫描历史", "Microsoft Defender Scan History", "C:\Documents and Settings\All Users\Application Data\Microsoft\Windows Defender\Scans","*.bin", 0);

insert into disk_scan_info values(10009, 100, "Windows_10_x86", 0, "系统自动更新日志文件", "System Auto Update log", "C:\Windows\SoftwareDistribution\DataStore","", 1);

insert into disk_scan_info values(10010, 100, "Windows_10_x86", 0, "Windows 系统日志文件", "Windows System Log Files", "C:\Windows\Panther","", 1);
insert into disk_scan_info values(10011, 100, "Windows_10_x86", 0, "Windows 系统日志文件", "Windows System Log Files", "C:\Windows\Logs","", 1);

insert into disk_scan_info values(10012, 100, "Windows_10_x86", 0, "IIS日志文件", "IIS Log Files", "C:\Windows\System32\LogFiles","", 1);

insert into disk_scan_info values(10013, 100, "Windows_10_x86", 0, "Intel 软件日志文件", "Intel Software Log Files", "C:\Intel\Logs","", 1);

insert into disk_scan_info values(10014, 100, "Windows_10_x86", 0, "微软Defender更新和备份缓存", "Microsoft Defender Upate and Backup Cache", "C:\ProgramData\Microsoft\Windows Defender\Definition Updates\Backup","", 1);
insert into disk_scan_info values(10015, 100, "Windows_10_x86", 0, "Windows WinSxs备份缓存", "Windows WinSxs Backup Cache", "C:\Windows\WinSxS\Backup","", 1);
insert into disk_scan_info values(10016, 100, "Windows_10_x86", 0, "manifast cashe缓存", "manifast cashe", "C:\Windows\WinSxS\ManifestCache","", 1);
insert into disk_scan_info values(10017, 100, "Windows_10_x86", 0, "缩略图缓存", "Thumb Cache", "%UserProfile%\AppData\Local\Microsoft\Windows\Explorer","", 1);
insert into disk_scan_info values(10018, 100, "Windows_10_x86", 0, "系统图标缓存", "System Icon Cache", "%UserProfile%\AppData\Local","IconCache.db", 0);
insert into disk_scan_info values(10019, 100, "Windows_10_x86", 0, "无效快捷方式", "Invalid shortcut", "%UserProfile%\AppData\Roaming\Microsoft\Windows\Recent","", 1);

insert into disk_scan_info values(10020, 100, "Windows_10_x86", 0, "谷歌浏览器缓存", "Chrome Cache", "%UserProfile%\AppData\Local\Google\Chrome\User Data\Default\Cache","", 1);
insert into disk_scan_info values(10021, 100, "Windows_10_x86", 0, "谷歌浏览器日志", "Chrome Cache", "%UserProfile%\AppData\Local\Google\Chrome\User Data\Default\Session Storage","*log*", 0);
insert into disk_scan_info values(10022, 100, "Windows_10_x86", 0, "IE浏览器缓存", "IE Cache", "%UserProfile%\AppData\Local\Microsoft\Windows\WebCache","", 1);
insert into disk_scan_info values(10023, 100, "Windows_10_x86", 0, "IE10以上浏览器缓存", "IE 10 Cache", "%UserProfile%\AppData\Local\Microsoft\Windows\INetCache\IE","", 1);
insert into disk_scan_info values(10024, 100, "Windows_10_x86", 0, "Edge浏览器缓存", "Edge Cache", "%UserProfile%\AppData\Local\Packages\Microsoft.MicrosoftEdge_8wekyb3d8bbwe\AC\MicrosoftEdge\Cache","", 1);
insert into disk_scan_info values(10025, 100, "Windows_10_x86", 0, "火狐浏览器缓存", "Firefox Cache", "%UserProfile%\AppData\Local\Mozilla\Firefox\Profiles","", 1);
insert into disk_scan_info values(10026, 100, "Windows_10_x86", 0, "火狐浏览器更新日志", "Firefox Update Cache", "%UserProfile%\AppData\Roaming\Mozilla\Firefox\Crash Reports","", 1);

-- windows 10 x64
insert into disk_scan_info values(10050, 100, "Windows_10_x64", 1, "Windows系统临时文件", "Windows System Temp Files", "%UserProfile%\AppData\Local\Temp","", 1);

-- ///////////////////////////////////////////////////////////////////////////////////////////////
-- //reg_scan_info
-- ///////////////////////////////////////////////////////////////////////////////////////////////
-- windows xp x86
insert into reg_scan_info values(5100, 51, "Windows_xp_x86", 0, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(5101, 51, "Windows_xp_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(5102, 51, "Windows_xp_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted");
insert into reg_scan_info values(5103, 51, "Windows_xp_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(5104, 51, "Windows_xp_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(5105, 51, "Windows_xp_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(5106, 51, "Windows_xp_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(5108, 51, "Windows_xp_x86", 0, 5, "错误的帮助信息", "Wrong Help Info", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Help");
insert into reg_scan_info values(5109, 51, "Windows_xp_x86", 0, 5, "错误的帮助信息", "Wrong Help Info", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Html Help");
insert into reg_scan_info values(5110, 51, "Windows_xp_x86", 0, 6, "错误的安装信息", "App Install Errors", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\Folders");
insert into reg_scan_info values(5111, 51, "Windows_xp_x86", 0, 7, "错误应用程序路径", "App Path Errors", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths");
insert into reg_scan_info values(5112, 51, "Windows_xp_x86", 0, 8, "残留的软件安装", "The residual software install", "HKEY_LOCAL_MACHINE\SOFTWARE");
insert into reg_scan_info values(5113, 51, "Windows_xp_x86", 0, 8, "残留的软件安装", "The residual software install", "HKEY_CURRENT_USER\Software");
insert into reg_scan_info values(5114, 51, "Windows_xp_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");

-- windows xp x64
insert into reg_scan_info values(5250, 52, "Windows_xp_x64", 1, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(5251, 52, "Windows_xp_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(5252, 52, "Windows_xp_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(5253, 52, "Windows_xp_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(5254, 52, "Windows_xp_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(5255, 52, "Windows_xp_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows vista x86
insert into reg_scan_info values(6000, 60, "Windows_vista_x86", 0, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6001, 60, "Windows_vista_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6002, 60, "Windows_vista_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6003, 60, "Windows_vista_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6004, 60, "Windows_vista_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6005, 60, "Windows_vista_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows vista x64
insert into reg_scan_info values(6050, 60, "Windows_vista_x64", 1, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6051, 60, "Windows_vista_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6052, 60, "Windows_vista_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6053, 60, "Windows_vista_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6054, 60, "Windows_vista_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6055, 60, "Windows_vista_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 7 x86
insert into reg_scan_info values(6100, 61, "Windows_7_x86", 0, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6101, 61, "Windows_7_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6102, 61, "Windows_7_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6103, 61, "Windows_7_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6104, 61, "Windows_7_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6105, 61, "Windows_7_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 7 x64
insert into reg_scan_info values(6150, 61, "Windows_7_x64", 1, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6151, 61, "Windows_7_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6152, 61, "Windows_7_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6153, 61, "Windows_7_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6154, 61, "Windows_7_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6155, 61, "Windows_7_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 8 x86
insert into reg_scan_info values(6200, 62, "Windows_8_x86", 0, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6201, 62, "Windows_8_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6202, 62, "Windows_8_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6203, 62, "Windows_8_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6204, 62, "Windows_8_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6205, 62, "Windows_8_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 8 x64
insert into reg_scan_info values(6250, 62, "Windows_8_x64", 1, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6251, 62, "Windows_8_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6252, 62, "Windows_8_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6253, 62, "Windows_8_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6254, 62, "Windows_8_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6255, 62, "Windows_8_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 8.1 x86
insert into reg_scan_info values(6300, 63, "Windows_81_x86", 0, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6301, 63, "Windows_81_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6302, 63, "Windows_81_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6303, 63, "Windows_81_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6304, 63, "Windows_81_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6305, 63, "Windows_81_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 8.1 x64
insert into reg_scan_info values(6350, 63, "Windows_81_x64", 1, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(6351, 63, "Windows_81_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(6352, 63, "Windows_81_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6353, 63, "Windows_81_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(6354, 63, "Windows_81_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(6355, 63, "Windows_81_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");

-- windows 10 x86
insert into reg_scan_info values(10000, 100, "Windows_10_x86", 0, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(10001, 100, "Windows_10_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(10002, 100, "Windows_xp_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Store");
insert into reg_scan_info values(10003, 100, "Windows_xp_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Persisted");
insert into reg_scan_info values(10004, 100, "Windows_10_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(10005, 100, "Windows_10_x86", 0, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(10006, 100, "Windows_10_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(10007, 100, "Windows_10_x86", 0, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(10008, 100, "Windows_10_x86", 0, 5, "错误的帮助信息", "Wrong Help Info", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Help");
insert into reg_scan_info values(10009, 100, "Windows_10_x86", 0, 5, "错误的帮助信息", "Wrong Help Info", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Html Help");
insert into reg_scan_info values(10010, 100, "Windows_10_x86", 0, 6, "错误的安装信息", "App Install Errors", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Installer\Folders");
insert into reg_scan_info values(10011, 100, "Windows_10_x86", 0, 7, "错误应用程序路径", "App Path Errors", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths");
insert into reg_scan_info values(10012, 100, "Windows_10_x86", 0, 8, "残留的软件安装", "The residual software install", "HKEY_LOCAL_MACHINE\SOFTWARE");
insert into reg_scan_info values(10013, 100, "Windows_10_x86", 0, 8, "残留的软件安装", "The residual software install", "HKEY_CURRENT_USER\Software");
insert into reg_scan_info values(10014, 100, "Windows_10_x86", 0, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");

-- windows 10 x64
insert into reg_scan_info values(10050, 100, "Windows_10_x64", 1, 1, "错误的DLL信息", "Invalid DLLs", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\SharedDlls");
insert into reg_scan_info values(10051, 100, "Windows_10_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers");
insert into reg_scan_info values(10052, 100, "Windows_10_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(10053, 100, "Windows_10_x64", 1, 3, "无效的启动项", "Invalid Startup Items", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run");
insert into reg_scan_info values(10054, 100, "Windows_10_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(10055, 100, "Windows_10_x64", 1, 4, "残留的软件卸载信息", "The residual software uninstall", "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Uninstall");
insert into reg_scan_info values(10056, 100, "Windows_xp_x64", 1, 2, "程序兼容性选项信息", "App Compat Flags", "HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Compatibility Assistant\Store");






