#include "opengl_hook.h"
#include <windows.h>
#include <gl/gl.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

using tSwapBuffers = BOOL(WINAPI*)(HDC);
tSwapBuffers oSwapBuffers = nullptr;

BOOL WINAPI hSwapBuffers(HDC hdc) {
    static bool init = false;
    if (!init) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplWin32_Init(WindowFromDC(hdc));
        ImGui_ImplOpenGL3_Init("#version 130");
        init = true;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return oSwapBuffers(hdc);
}

void OpenGLHook::Install() {
    HMODULE hOpenGL = GetModuleHandleA("opengl32.dll");
    if (hOpenGL) {
        oSwapBuffers = (tSwapBuffers)GetProcAddress(hOpenGL, "wglSwapBuffers");
    }
}
void OpenGLHook::Uninstall() { }
