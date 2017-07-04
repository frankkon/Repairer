#ifndef TOOL_H
#define TOOL_H

#include <QString>

#include <Windows.h>
#include <ShellAPI.h>

typedef BOOL (WINAPI *PWOW64)(PVOID);

class Tool
{
public:
    Tool();

    static void openAndSelectFile( QString strFile );      //打开并选择文件
    static bool isFileOrDirExist( QString strFile );       //判断文件或目录是否存在
    static void deleteFile( QString strFile );             //删除文件
    static void reomveDir( QString strPath );              //删除文件夹下所有文件
    static void removeFilesinDir(const QString path);
    static void createDirectory( QString strPath, bool bFile );        //创建路径,父目录不存在时先创建父路径 strPath可以为文件或者文件夹路径

    static QString getFileName( QString strFullPath );           //得到文件名(包含扩展名)
    static QString getFileNameWithoutSuffix( QString strFile );  //得到文件名(不包括扩展名)
    static QString getFileSuffix( QString strName );             //得到文件扩展名,参数可以是( c:\a.dll 或 a.dll )

    static void uncompressFile( QString strCompressedFile, QString strSavePath, QString str7zPath );  //解压缩文件

    static QString getFileVersion( QString strFile );  //获得文件版本号
    static int getFileSize( QString strFile );  //获得文件大小 单位kb
    static qint64 getFileSizeByte( QString strFile );  //文件大小 byte
    static bool isFileTypeX86( QString strFile );   //32位返回true，64位返回false

    static QString getSystemInfo();

private:
    static void createSubDirectory( QString strPath );     //创建文件夹，父目录必须存在

    static QString queryValue( QString strValueName, DWORD dwLangCharset, BYTE* pVersionData );
};

#endif // TOOL_H
