#include "theme.h"
namespace ThemeManager {
    Theme current;
    std::vector<Theme> presets;

    void Init() {
        presets.push_back({"White", true, {0.28f, 0.33f, 0.41f, 1.0f}, {0.39f, 0.46f, 0.55f, 1.0f}, {0.12f, 0.16f, 0.23f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.94f}, {0.98f, 0.98f, 0.98f, 1.0f}, {0.96f, 0.96f, 0.96f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.09f, 0.09f, 0.11f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.32f, 0.32f, 0.36f, 1.0f}, {0.44f, 0.44f, 0.48f, 1.0f}, {0.63f, 0.63f, 0.67f, 1.0f}, {0,0,0,0.06f}, {0,0,0,0.15f}});
        presets.push_back({"Dark", false, {0.38f, 0.65f, 0.98f, 1.0f}, {0.58f, 0.77f, 0.99f, 1.0f}, {0.23f, 0.51f, 0.96f, 1.0f}, {0.06f, 0.09f, 0.16f, 0.95f}, {0.04f, 0.06f, 0.12f, 1.0f}, {0.12f, 0.16f, 0.23f, 1.0f}, {0.12f, 0.16f, 0.23f, 1.0f}, {0.89f, 0.91f, 0.94f, 1.0f}, {0.97f, 0.98f, 0.99f, 1.0f}, {0.80f, 0.83f, 0.88f, 1.0f}, {0.58f, 0.64f, 0.71f, 1.0f}, {0.39f, 0.45f, 0.54f, 1.0f}, {0,0,0,0.4f}, {0,0,0,0.6f}});
        presets.push_back({"Midnight", false, {0.55f, 0.36f, 0.96f, 1.0f}, {0.65f, 0.55f, 0.98f, 1.0f}, {0.43f, 0.16f, 0.85f, 1.0f}, {0.05f, 0.04f, 0.12f, 0.95f}, {0.03f, 0.02f, 0.09f, 1.0f}, {0.10f, 0.09f, 0.21f, 1.0f}, {0.10f, 0.09f, 0.21f, 1.0f}, {0.91f, 0.89f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.77f, 0.71f, 0.99f, 1.0f}, {0.65f, 0.55f, 0.98f, 1.0f}, {0.49f, 0.23f, 0.93f, 1.0f}, {0,0,0,0.5f}, {0,0,0,0.7f}});
        current = presets[0];
    }

    void Apply(const Theme& t) {
        current = t;
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = t.winBg;
        colors[ImGuiCol_ChildBg] = t.panelBg;
        colors[ImGuiCol_PopupBg] = t.winBg;
        colors[ImGuiCol_Border] = ImVec4(t.text.x, t.text.y, t.text.z, 0.08f);
        colors[ImGuiCol_FrameBg] = t.inputBg;
        colors[ImGuiCol_FrameBgHovered] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.1f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.2f);
        colors[ImGuiCol_TitleBg] = t.sidebarBg;
        colors[ImGuiCol_TitleBgActive] = t.sidebarBg;
        colors[ImGuiCol_ScrollbarBg] = t.panelBg;
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(t.text.x, t.text.y, t.text.z, 0.2f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.5f);
        colors[ImGuiCol_ScrollbarGrabActive] = t.accent;
        colors[ImGuiCol_CheckMark] = t.accent;
        colors[ImGuiCol_SliderGrab] = t.accent;
        colors[ImGuiCol_SliderGrabActive] = t.accentBright;
        colors[ImGuiCol_Button] = t.inputBg;
        colors[ImGuiCol_ButtonHovered] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.15f);
        colors[ImGuiCol_ButtonActive] = t.accent;
        colors[ImGuiCol_Header] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.1f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.2f);
        colors[ImGuiCol_HeaderActive] = ImVec4(t.accent.x, t.accent.y, t.accent.z, 0.3f);
        colors[ImGuiCol_Text] = t.text;
        style.WindowRounding = 10.0f;
        style.ChildRounding = 8.0f;
        style.FrameRounding = 5.0f;
        style.PopupRounding = 8.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 4.0f;
        style.WindowPadding = ImVec2(0, 0);
        style.FramePadding = ImVec2(8, 4);
        style.ItemSpacing = ImVec2(8, 8);
    }

    ImU32 GetAccentColor() {
        return ImGui::ColorConvertFloat4ToU32(current.accent);
    }
}