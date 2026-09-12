#include "bypass.h"
#include <winternl.h>
#include <psapi.h>

namespace Bypass {
    void Init() { 
        HideModule(GetModuleHandleA(NULL)); 
        AntiDebug();
        AntiDump();
    }
    void AntiDebug() {
        if (IsDebuggerPresent() || CheckRemoteDebuggerPresent(GetCurrentProcess(), 0)) ExitProcess(0);
    }
    void AntiDump() {
        MODULEINFO mi;
        GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mi, sizeof(mi));
        DWORD oldProtect;
        VirtualProtect(mi.lpBaseOfDll, 0x1000, PAGE_READWRITE, &oldProtect);
        memset(mi.lpBaseOfDll, 0, 0x1000);
        VirtualProtect(mi.lpBaseOfDll, 0x1000, oldProtect, &oldProtect);
    }
    void HideModule(HMODULE hMod) {
        // Unlink from PEB LDR
    }
}
