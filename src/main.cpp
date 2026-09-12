// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include <windows.h>
#include <thread>
#include <chrono>
#include "gui/menu.h"
#include "render/opengl_hook.h"
#include "core/il2cpp_bridge.h"
#include "features/registry.h"
#include "bypass/bypass.h"

// Input handling thread for menu
DWORD WINAPI InputThread(LPVOID) {
    while (true) {
        // VK_RBUTTON (Right Mouse Button) to OPEN menu
        if (GetAsyncKeyState(VK_RBUTTON) & 1) {
            VV::GUI::Menu::s_isOpen = true;
        }
        
        // VK_ESCAPE to CLOSE menu
        if (GetAsyncKeyState(VK_ESCAPE) & 1) {
            VV::GUI::Menu::s_isOpen = false;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}

DWORD WINAPI MainThread(LPVOID) {
    // Initialize Bypass Engine first!
    VV::Bypass::CoreBypass::Get().Initialize();

    // Wait for GameAssembly or JVM (if it's a Unity or Java game)
    while (!GetModuleHandleA("GameAssembly.dll") && !GetModuleHandleA("jvm.dll")) {
        Sleep(200);
    }
    
    // Give the game some time to fully initialize
    Sleep(3000);
    
    VV::Core::IL2CPPBridge::Init();
    VV::Features::Registry::Init();
    VV::Render::OpenGLHook::Install();
    VV::GUI::Menu::Init();
    
    // Start input handler
    CreateThread(nullptr, 0, InputThread, nullptr, 0, nullptr);
    
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        // Erase PE headers immediately upon injection
        DWORD oldProt = 0;
        VirtualProtect((LPVOID)hMod, 0x1000, PAGE_EXECUTE_READWRITE, &oldProt);
        memset((LPVOID)hMod, 0, 0x1000);
        VirtualProtect((LPVOID)hMod, 0x1000, oldProt, &oldProt);
        
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    }
    return TRUE;
}
