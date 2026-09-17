// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "menu.h"
#include "widgets.h"
#include "theme.h"
#include "imgui.h"
#include <windows.h>

namespace VV::GUI {

bool Menu::s_isOpen = false;
static int s_currentTab = 0;
static bool s_initialized = false;

void Menu::Init() {
    if (s_initialized) return;
    Theme::Apply(Theme::Dark);
    s_initialized = true;
}

bool Menu::IsOpen() { return s_isOpen; }
void Menu::SetOpen(bool open) { s_isOpen = open; }

static void HandleInput() {
    static bool previousToggle = false;
    const bool pressed = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
    if (pressed && !previousToggle) Menu::s_isOpen = !Menu::s_isOpen;
    previousToggle = pressed;

    if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0) {
        Menu::s_isOpen = false;
    }
}

void Menu::Render() {
    HandleInput();
    if (!s_isOpen) return;

    if (ImGui::Begin("STALZONE", &s_isOpen)) {
        constexpr const char* tabs[] = {"Combat", "Visual", "Settings"};
        for (int i = 0; i < 3; ++i) {
            if (ImGui::Selectable(tabs[i], s_currentTab == i, 0, ImVec2(100.0f, 0.0f))) {
                s_currentTab = i;
            }
            if (i < 2) ImGui::SameLine();
        }
        ImGui::Separator();
        if (s_currentTab == 2) {
            ImGui::TextUnformatted("Settings are stored locally by the application.");
        } else {
            ImGui::TextUnformatted("Feature backend is not configured.");
            ImGui::TextUnformatted("No game-process or undocumented API integration is enabled.");
        }
    }
    ImGui::End();
}

} // namespace VV::GUI
