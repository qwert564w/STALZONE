#include <windows.h>
#include <thread>
#include "gui/menu.h"
#include "gui/theme.h"
#include "render/opengl_hook.h"

DWORD WINAPI MainThread(LPVOID) {
    while (!GetModuleHandleA("GameAssembly.dll")) Sleep(200);
    Sleep(5000);
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