#pragma once
#include <imgui.h>
#include <string>
#include <vector>

struct Theme {
    std::string name;
    bool light;
    ImVec4 accent, accentBright, accentDark;
    ImVec4 winBg, sidebarBg, panelBg, inputBg;
    ImVec4 text, textBright, textMid, textDim, textFaint;
    ImVec4 shadow, shadowStrong;
};

namespace ThemeManager {
    extern Theme current;
    extern std::vector<Theme> presets;
    void Init();
    void Apply(const Theme& t);
    ImU32 GetAccentColor();
}