#pragma once
#include <windows.h>
namespace ManualMap {
    bool Inject(HANDLE hProc, const char* dllPath);
    bool Unload(HANDLE hProc);
}