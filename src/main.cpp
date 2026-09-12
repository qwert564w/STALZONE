// VV.EXE — entry point
#include <windows.h>
#include <thread>
#include "core/il2cpp_bridge.h"
#include "core/manual_map.h"
#include "render/opengl_hook.h"
#include "gui/menu.h"
#include "features/features.h"

DWORD WINAPI MainThread(LPVOID) {
    while (!GetModuleHandleA("GameAssembly.dll")) Sleep(200);
    Sleep(5000);
    if (!IL2CPP::Init()) return 0;
    OpenGLHook::Install();
    Features::RegisterAll();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    }
    return TRUE;
}