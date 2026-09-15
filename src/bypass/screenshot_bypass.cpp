#include "screenshot_bypass.h"
#include <windows.h>
#include <detours.h> // Standard hooking library, fallback to manual inline hook if unavailable

namespace ScreenshotBypass {
    typedef BOOL(WINAPI* tBitBlt)(HDC, int, int, int, int, HDC, int, int, DWORD);
    tBitBlt oBitBlt = nullptr;
    
    // Trampoline buffer for manual inline hook
    unsigned char g_trampoline[14] = {0};
    bool g_hooked = false;

    BOOL WINAPI hBitBlt(HDC hdcDest, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) {
        // Screenshot bypass logic:
        // When an anti-cheat attempts to capture the screen via BitBlt, 
        // we can temporarily hide cheat windows or overlay elements.
        
        // Example: Hide the cheat menu window if it exists
        // HWND cheatWnd = FindWindowA("CheatWindowClass", nullptr);
        // if (cheatWnd) ShowWindow(cheatWnd, SW_HIDE);
        
        BOOL result = oBitBlt(hdcDest, x, y, cx, cy, hdcSrc, x1, y1, rop);
        
        // Restore visibility after capture
        // if (cheatWnd) ShowWindow(cheatWnd, SW_SHOW);
        
        return result;
    }

    void Init() {
        if (g_hooked) return;
        
        HMODULE hGdi32 = GetModuleHandleA("gdi32.dll");
        if (!hGdi32) hGdi32 = LoadLibraryA("gdi32.dll");
        if (!hGdi32) return;

        oBitBlt = (tBitBlt)GetProcAddress(hGdi32, "BitBlt");
        if (!oBitBlt) return;

        // Manual x64 inline hook: mov r11, <address>; jmp r11; ret
        DWORD oldProtect;
        if (VirtualProtect((LPVOID)oBitBlt, 14, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            // Save original bytes for potential unhooking
            memcpy(g_trampoline, oBitBlt, 14);
            
            unsigned char jmpInstruction[14] = { 0x49, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0xFF, 0xE3, 0xC3 };
            *(uintptr_t*)(jmpInstruction + 2) = (uintptr_t)hBitBlt;
            
            memcpy((LPVOID)oBitBlt, jmpInstruction, 14);
            VirtualProtect((LPVOID)oBitBlt, 14, oldProtect, &oldProtect);
            g_hooked = true;
        }
    }

    void Cleanup() {
        if (!g_hooked || !oBitBlt) return;
        
        DWORD oldProtect;
        if (VirtualProtect((LPVOID)oBitBlt, 14, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            memcpy((LPVOID)oBitBlt, g_trampoline, 14);
            VirtualProtect((LPVOID)oBitBlt, 14, oldProtect, &oldProtect);
            g_hooked = false;
        }
    }
}
