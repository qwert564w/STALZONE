#pragma once
#include <imgui.h>
#include <string>
#include <vector>
struct Theme { std::string name; bool light; ImVec4 accent, accentBright, accentDark; ImVec4 winBg, sidebarBg, panelBg, inputBg; ImVec4 border, borderStrong; ImVec4 text, textBright, textMid, textDim, textFaint; ImVec4 shadow, shadowStrong; };
namespace ThemeManager { extern Theme current; extern std::vector<Theme> presets; extern std::vector<Theme> custom; extern int currentIndex; void LoadPresets(); void Apply(const Theme& t); void ApplyByIndex(int idx); void AddCustom(const Theme& t); void RemoveCustom(int idx); void Reset(); void Save(const char* path); void Load(const char* path); }