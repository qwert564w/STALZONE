#pragma once
#include <windows.h>
namespace OpenGLHook {
    bool Install();
    void Uninstall();
    HWND GetGameWindow();
    void GetViewportSize(int* w, int* h);
}