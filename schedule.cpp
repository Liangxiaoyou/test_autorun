#include <windows.h>
#include <initguid.h>
#include <ole2.h>
#include <mstask.h>
#include <msterr.h>
#include <wchar.h>

#define TASKS_TO_RETRIEVE          5


int main(int argc, char **argv)
{
  HRESULT hr = S_OK;
  ITaskScheduler *pITS;
  
  
  /////////////////////////////////////////////////////////////////
  // Call CoInitialize to initialize the COM library and 
  // then call CoCreateInstance to get the Task Scheduler object. 
  /////////////////////////////////////////////////////////////////
  hr = CoInitialize(NULL);
  if (SUCCEEDED(hr))
  {
    hr = CoCreateInstance(CLSID_CTaskScheduler,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ITaskScheduler,
                          (void **) &pITS);
    if (FAILED(hr))
    {
      CoUninitialize();
      return hr;
    }
  }
  else
  {
    return hr;
  }
  
  /////////////////////////////////////////////////////////////////
  // Call ITaskScheduler::Enum to get an enumeration object.
  /////////////////////////////////////////////////////////////////
  IEnumWorkItems *pIEnum;
  hr = pITS->Enum(&pIEnum);
  pITS->Release();
  if (FAILED(hr))
  {
    CoUninitialize();
    return hr;
  }
  
  /////////////////////////////////////////////////////////////////
  // Call IEnumWorkItems::Next to retrieve tasks. Note that 
  // this example tries to retrieve five tasks for each call.
  /////////////////////////////////////////////////////////////////
  LPWSTR *lpwszNames;
  DWORD dwFetchedTasks = 0;
  while (SUCCEEDED(pIEnum->Next(TASKS_TO_RETRIEVE,
                                &lpwszNames,
                                &dwFetchedTasks))
                  && (dwFetchedTasks != 0))
  {
    ///////////////////////////////////////////////////////////////
    // Process each task. Note that this example prints the 
    // name of each task to the screen.
    //////////////////////////////////////////////////////////////
    while (dwFetchedTasks)
    {
       wprintf(L"%s\n", lpwszNames[--dwFetchedTasks]);
       CoTaskMemFree(lpwszNames[dwFetchedTasks]);
    }
    CoTaskMemFree(lpwszNames);
  }
  
  pIEnum->Release();
  CoUninitialize();
  return S_OK;
}