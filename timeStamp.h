#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <cstring>
#include <iostream>


//https://docs.microsoft.com/en-us/windows/win32/sysinfo/retrieving-the-last-write-time
// GetLastWriteTime - Retrieves the last-write time and converts
//                    the time to a string
//
// Return value - TRUE if successful, FALSE otherwise
// hFile      - Valid file handle
// lpszString - Pointer to buffer to receive string

BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    DWORD dwRet;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return FALSE;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
    dwRet = StringCchPrintf(lpszString, dwSize, 
        TEXT("%02d/%02d/%d  %02d:%02d"),
        stLocal.wMonth, stLocal.wDay, stLocal.wYear,
        stLocal.wHour, stLocal.wMinute);

    if( S_OK == dwRet )
        return TRUE;
    else return FALSE;
}

bool  get_time_stamp(char * path,char* timestamp)
{
    HANDLE hFile;
    TCHAR szBuf[MAX_PATH];

    // if( argc != 2 )
    // {
    //     printf("This sample takes a file name as a parameter\n");
    //     return 0;
    // }
    hFile = CreateFile("C:\\Windows\\system32\\SecurityHealthSystray.exe", GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, 0, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile failed with %d\n", GetLastError());
        return false;
    }
    if(GetLastWriteTime( hFile, szBuf, MAX_PATH )){
        _tprintf(TEXT("Last write time is: %s\n"), szBuf);
        int i=0;
        for(;;i++){
            if(szBuf[i]=='\0') break;
            timestamp[i] = szBuf[i];
        }
        timestamp[i] = '\0';
        //strcpy(timestamp,szBuf);
    }
    CloseHandle(hFile);    
    return true;
}

// int main(){
//     char * path ="C:\\Windows\\system32\\SecurityHealthSystray.exe";
//     char timestamp[1024];
//     get_time_stamp(path,timestamp);
//     std::cout<<"the timestamp is "<<timestamp<<std::endl;
// }