#include <windows.h>
#include <thread>
#include "gui/menu.h"
#include "gui/theme.h"
#include "render/opengl_hook.h"
#include "core/il2cpp_bridge.h"
#include "features/registry.h"

DWORD WINAPI MainThread(LPVOID) {
    while (!GetModuleHandleA("GameAssembly.dll") && !GetModuleHandleA("jvm.dll")) Sleep(200);
    Sleep(2000);
    IL2CPPBridge::Init();
    Features::Registry::Init();
    OpenGLHook::Install();
    Menu::Init();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    }
    return TRUE;
}
