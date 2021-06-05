// QueryKey - Enumerates the subkeys of key and its associated values.
//     hKey - Key whose subkeys and values are to be enumerated.

#include "mytable.h"

using namespace std;



void byte2charx(BYTE *a,int size,char* b){
    int i=0;
    for(;i<size;i++){
        b[i] = a[i];
        //cout<<"a["<<i<<"]="<<a[i]<<endl;
    }
    b[i]='\0';
}

 
void QueryKey(HKEY hKey,mytable *tbl) 
{ 
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues=0;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD i, retCode; 
 
    TCHAR  achValue[MAX_VALUE_NAME]; 
    DWORD cchValue = MAX_VALUE_NAME; 

    LPDWORD lpType;
    LPBYTE  lpData ;
    LPDWORD lpcbData;
    BYTE  achData[MAX_VALUE_NAME];
    DWORD cchData = MAX_VALUE_NAME;
    DWORD type;
    // Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 

        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
    cout<<"cSubKeys is "<<cSubKeys<<endl;
    printf("cSubKeys is %d\n",cSubKeys);
    printf("cValues is %d\n",cValues);
    cout<<"cValues is "<<cValues<<endl;
    // Enumerate the subkeys, until RegEnumKeyEx fails.
    /*
    if (cSubKeys)
    {
        printf( "\nNumber of subkeys: %d\n", cSubKeys);

        for (i=0; i<cSubKeys; i++) 
        { 
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyEx(
                     hKey, 
                     i,
                     achKey, 
                     &cbName, //返回achkey的大小,字节为单位
                     NULL, 
                     NULL, 
                     NULL, 
                     &ftLastWriteTime); 
            if (retCode == ERROR_SUCCESS) 
            {
                _tprintf(TEXT("(%d) %s\n"), i+1, achKey);
            }
        }
    } 
    */
    // Enumerate the key values. 
    if (cValues) 
    {
        printf( "\nNumber of values: %d\n", cValues);

        for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
        { 
            cchValue = MAX_VALUE_NAME; 
            cchData = MAX_VALUE_NAME;
            achValue[0] = '\0';
            achData[0] = '\0';
            //cout<<"@"<<endl;//如果不加这个endl,那么就不会显示打印的结果
            retCode = RegEnumValue(
                hKey,
                i, 
                achValue, 
                &cchValue, 
                NULL, 
                &type,
                achData,
                &cchData
                );
            //cout<<"@"<<endl;
            if (retCode == ERROR_SUCCESS) 
            { 
                _tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
                if(achData == NULL) cout<<"lpData is NULL\n";
                else {
                    cout<<"achdata type is "<<type<<endl;
                    cout<<"lpData is :"<<achData<<"."<<endl;
                    char* result;
                    result = new char[cchData];
                    cout<<"@1"<<endl;
                    byte2charx(achData,cchData,result);
                    cout<<"result is "<<result<<endl;

                    tbl->appendRow(achValue,"",result,"","");
                    }
            }
            else {
                cout<<"("<<i+1<<") read key value "<< "error\nerror code is "<<retCode<<"\nwe need the achdata size is"<<cchData<<endl;
                cout<<"\nwe need the achValue size is"<<cchValue<<endl;
            }

        }
    }

}

int main()
{
    mytable tbl;
    HKEY hTestKey;
    char* path[5] = {"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer\\Run",
                    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
                    "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunOnceEx",
                    "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"};
    for(int i=0;i<5;i++){
        if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                TEXT(path[i]),
                0,
                KEY_READ,
                &hTestKey) == ERROR_SUCCESS
            )
        {
            QueryKey(hTestKey,&tbl);
            RegCloseKey(hTestKey);
        }
        else cout<<"open HKML fail!->"<<path[i]<<endl;

        if( RegOpenKeyEx( HKEY_CURRENT_USER,
                TEXT(path[i]),
                0,
                KEY_READ,
                &hTestKey) == ERROR_SUCCESS
            )
        {
            QueryKey(hTestKey,&tbl);
            RegCloseKey(hTestKey);
        }
        else cout<<"open HKCU fail!->"<<path[i]<<endl;
    }
    tbl.printTable();
   cout<<"process end"<<endl;
}