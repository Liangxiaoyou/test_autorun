// conGetVer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include "pch.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cstring>

//  pName可取值：
//    _T("CompanyName"),_T("FileDescription"),_T("FileVersion"),
//  _T("InternalName"),_T("LegalCopyright"),_T("OriginalFilename"),
//  _T("ProductName"),_T("ProductVersion"),
BOOL GetFileVersionString(LPCTSTR pFileName, LPCTSTR pName /* = NULL */, LPTSTR ptBuf, UINT lenBuf)
{
    DWORD   dwDummyHandle = 0; // will always be set to zero
    DWORD   dwLen = 0;
    BYTE*    pVersionInfo = NULL;
    BOOL    bRetVal;

    VS_FIXEDFILEINFO    FileVersion;

    HMODULE        hVerDll;
    hVerDll = LoadLibrary(_T("VERSION.dll"));
    if (hVerDll == NULL)
        return FALSE;

#ifdef _UNICODE
    typedef DWORD(WINAPI *Fun_GetFileVersionInfoSize)(LPCTSTR, DWORD *);
    typedef BOOL(WINAPI *Fun_GetFileVersionInfo)(LPCTSTR, DWORD, DWORD, LPVOID);
    typedef BOOL(WINAPI *Fun_VerQueryValue)(LPCVOID, LPCTSTR, LPVOID, PUINT);
#else
    typedef DWORD(WINAPI *Fun_GetFileVersionInfoSize)(LPCSTR, DWORD *);
    typedef BOOL(WINAPI *Fun_GetFileVersionInfo)(LPCSTR, DWORD, DWORD, LPVOID);
    typedef BOOL(WINAPI *Fun_VerQueryValue)(LPCVOID, LPCSTR, LPVOID, PUINT);
#endif

    Fun_GetFileVersionInfoSize        pGetFileVersionInfoSize;
    Fun_GetFileVersionInfo            pGetFileVersionInfo;
    Fun_VerQueryValue                pVerQueryValue;

#ifdef _UNICODE
    pGetFileVersionInfoSize = (Fun_GetFileVersionInfoSize)::GetProcAddress(hVerDll, "GetFileVersionInfoSizeW");
    pGetFileVersionInfo = (Fun_GetFileVersionInfo)::GetProcAddress(hVerDll, "GetFileVersionInfoW");
    pVerQueryValue = (Fun_VerQueryValue)::GetProcAddress(hVerDll, "VerQueryValueW");
#else
    pGetFileVersionInfoSize = (Fun_GetFileVersionInfoSize)::GetProcAddress(hVerDll, "GetFileVersionInfoSizeA");
    pGetFileVersionInfo = (Fun_GetFileVersionInfo)::GetProcAddress(hVerDll, "GetFileVersionInfoA");
    pVerQueryValue = (Fun_VerQueryValue)::GetProcAddress(hVerDll, "VerQueryValueA");
#endif

    struct TRANSLATION {
        WORD langID;            // language ID
        WORD charset;            // character set (code page)
    } Translation;

    Translation.langID = 0x0409;    //
    Translation.charset = 1252;        // default = ANSI code page

    dwLen = pGetFileVersionInfoSize(pFileName, &dwDummyHandle);
    if (dwLen == 0)
    {
        bRetVal = FALSE;
        goto End;
    }

    pVersionInfo = new BYTE[dwLen]; // allocate version info
    bRetVal = pGetFileVersionInfo(pFileName, 0, dwLen, pVersionInfo);
    if (bRetVal == FALSE)
    {
        goto End;
    }

    VOID    * pVI;
    UINT    uLen;

    bRetVal = pVerQueryValue(pVersionInfo, _T("\\"), &pVI, &uLen);
    if (bRetVal == FALSE)
    {
        goto End;
    }

    memcpy(&FileVersion, pVI, sizeof(VS_FIXEDFILEINFO));

    bRetVal = pVerQueryValue(pVersionInfo, _T("\\VarFileInfo\\Translation"),
        &pVI, &uLen);
    if (bRetVal && uLen >= 4)
    {
        memcpy(&Translation, pVI, sizeof(TRANSLATION));
    }
    else
    {
        bRetVal = FALSE;
        goto End;
    }

    //  BREAKIF(FileVersion.dwSignature != VS_FFI_SIGNATURE);
    if (FileVersion.dwSignature != VS_FFI_SIGNATURE)
    {
        bRetVal = FALSE;
        goto End;
    }

    VOID        *pVal;
    UINT        iLenVal;

    if (pName == NULL)
    {
        _stprintf_s(ptBuf, lenBuf, _T("%d.%d.%d.%d"),
            HIWORD(FileVersion.dwFileVersionMS), LOWORD(FileVersion.dwFileVersionMS),
            HIWORD(FileVersion.dwFileVersionLS), LOWORD(FileVersion.dwFileVersionLS));
    }
    else
    {
        TCHAR    szQuery[1024];
        _stprintf_s(szQuery, 1024, _T("\\StringFileInfo\\%04X%04X\\%s"),
            Translation.langID, Translation.charset, pName);

        bRetVal = pVerQueryValue(pVersionInfo, szQuery, &pVal, &iLenVal);
        if (bRetVal)
        {
            _stprintf_s(ptBuf, lenBuf, _T("%s"), (TCHAR*)pVal);
        }
        else
        {
            _stprintf_s(ptBuf, lenBuf, _T("%s"), _T(""));
        }
    }

End:
    FreeLibrary(hVerDll);
    hVerDll = NULL;
    delete[] pVersionInfo;
    pVersionInfo = NULL;

    return bRetVal;
}


void get_file_info(char* path,char * key,char* result)
{
    TCHAR* ptszStr = NULL;

    ptszStr = new TCHAR[1024];
    BOOL bRet = GetFileVersionString(_T(path),
        _T(key), ptszStr, 1024);

    //std::cout<<ptszStr<<std::endl;
    strcpy(result,ptszStr);
    //std::cout<<result<<std::endl;

    delete ptszStr;

}



string format_image_path(string image_path) 
{
    int n;
    if ((n = image_path.find(" -")) != string::npos)    // 含有命令选项
        image_path = image_path.substr(0, n);
    if ((n = image_path.find(" /")) != string::npos)    // 含有命令选项
        image_path = image_path.substr(0, n);
    if ((n = image_path.find("\"")) != string::npos){    // 被双引号包裹的路径
        if (n == 0){
            image_path = image_path.substr(1);
            if ((n = image_path.find("\"")) != string::npos)
                image_path = image_path.substr(0, n);
        }
    }
    if ((n = image_path.find("system32")) != string::npos)   // 包含系统环境变量
        image_path = image_path.substr(n).insert(0, "C:\\Windows\\");
    if ((n = image_path.find("System32")) != string::npos)
        image_path = image_path.substr(n).insert(0, "C:\\Windows\\");
    if ((n = image_path.find("SysWOW64\\")) != string::npos)
        image_path = image_path.substr(n).insert(0, "C:\\Windows\\");
    if ((n = image_path.find("\\??\\")) != string::npos)
        image_path = image_path.substr(4);
    if ((n = image_path.find("%ProgramFiles%")) != string::npos)
        image_path = image_path.substr(14).insert(0, "C:\\Program Files");
    if ((n = image_path.find("%PROGRAMFILES%")) != string::npos)
        image_path = image_path.substr(14).insert(0, "C:\\Program Files");
    if ((n = image_path.find("%ProgramData%")) != string::npos)
        image_path = image_path.substr(13).insert(0, "C:\\ProgramData");
    if ((n = image_path.find("%SystemRoot%")) != string::npos)
        image_path = image_path.substr(12).insert(0, "C:\\Windows");
    if ((n = image_path.find("\\")) == string::npos)
        image_path = image_path.insert(0, "C:\\Windows\\System32\\");
    return image_path;
}


void getPath(char* path,char* result){
    int n=0;
    int j=0;
    bool end = false;

    if(path[0] == '"') j=1;
    for(;;j++){
        switch(path[j]){
            case ',':end = true;break;
            case '"':end = true;break;
            case '\0':end = true;break;
            default: result[n++]=path[j];break;
        }
        if(end==true){
            result[n++] = '\0';
            break;}
    }
    string result1 =format_image_path(result);
    strcpy(result,result1.c_str());
}
//  pName可取值：
//    _T("CompanyName"),_T("FileDescription"),_T("FileVersion"),
//  _T("InternalName"),_T("LegalCopyright"),_T("OriginalFilename"),
//  _T("ProductName"),_T("ProductVersion"),
// int main(){
//     char* path ="c:\\program files\\dell\\dellmobileconnectdrivers\\dellmobileconnectwelcome.exe";
//     char* key ="ProductVersion";
//     //要在进入函数之前准备好空间，否则返回的时候不会成功，
//     char* result=  new char[1024];
//     if(result == NULL)
//        std::cout <<"is null!\n"; 
//     get_file_info(path,key,result);
//     if(result == NULL)
//        std::cout <<"is null!\n"; 
//     std::cout <<"Hello World!\n"; 
//     std::cout<<result<<std::endl;

// }