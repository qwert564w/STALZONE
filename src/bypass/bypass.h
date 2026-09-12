#pragma once
#include <windows.h>
namespace Bypass {
    void Init();
    void HideModule(HMODULE hMod);
    void AntiDebug();
    void AntiDump();
}
