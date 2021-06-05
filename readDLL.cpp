#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <shlwapi.h>
#include <iostream>
using namespace std;
//HRESULT SHLoadIndirectString(
//  _In_        PCWSTR pszSource,
//  _Out_       PWSTR pszOutBuf,
//  _In_        UINT cchOutBuf,
//  _Reserved_  void **ppvReserved
//);
int ReadDllDescription(TCHAR* szOut, PCWSTR PATH)
{
    if (SHLoadIndirectString(PATH,  szOut, 1024, NULL) == S_OK)
        return 1;
    return 0;
}

int main(){
    TCHAR szout[1024];
    char* path = "C:\\Program Files\\Dell\\DellMobileConnectDrivers\\DellMobileConnectWelcome.exe";
    ReadDllDescription(szout,TEXT("C:\\Program Files\\Dell\\DellMobileConnectDrivers\\DellMobileConnectWelcome.exe"));
    cout<<"szout is"<<szout<<endl;
    return 1;
}
