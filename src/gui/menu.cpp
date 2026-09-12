#include "menu.h"
#include "theme.h"
#include "particles.h"
#include "widgets.h"
#include <imgui_internal.h>
namespace Menu {
    int currentTab = 0;
    bool visible = true;
    static Particles::Config particlesCfg;
    static bool featureExpanded[64] = {false};
    void Init() {
        ThemeManager::Init();
        ThemeManager::Apply(ThemeManager::current);
        Particles::Init(particlesCfg);
    }
    void RenderSidebar() {
        const char* tabs[] = {"Combat", "Visual", "Players", "Auction", "World", "Misc", "Config", "Theme"};
        ImGui::BeginChild("##sidebar", ImVec2(140, 0), true, ImGuiWindowFlags_NoScrollbar);
        ImVec2 start_pos = ImGui::GetWindowPos();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(start_pos, ImVec2(start_pos.x + 140, start_pos.y + ImGui::GetWindowHeight()), ImGui::ColorConvertFloat4ToU32(ThemeManager::current.sidebarBg));
        ImGui::SetCursorPos(ImVec2(0, 10));
        for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
            bool is_active = (currentTab == i);
            if (is_active) {
                ImVec2 p = ImGui::GetCursorScreenPos();
                dl->AddRectFilled(ImVec2(start_pos.x, p.y), ImVec2(start_pos.x + 140, p.y + 32), ImVec4(ThemeManager::current.accent.x, ThemeManager::current.accent.y, ThemeManager::current.accent.z, 0.14f));
                dl->AddRectFilled(ImVec2(start_pos.x, p.y), ImVec2(start_pos.x + 3, p.y + 32), ImGui::ColorConvertFloat4ToU32(ThemeManager::current.accent));
            }
            if (is_active) ImGui::PushStyleColor(ImGuiCol_Text, ThemeManager::current.accent);
            ImGui::SetCursorPosX(20);
            if (ImGui::Selectable(tabs[i], false, 0, ImVec2(100, 32))) { currentTab = i; }
            if (is_active) ImGui::PopStyleColor();
        }
        ImGui::EndChild();
    }
    void RenderCombat() {
        Widgets::SectionGroup("Прицеливание");
        static bool aimbotEnabled = false, aimbotHeadOnly = true, aimbotVisCheck = true;
        static float aimbotFov = 15.0f, aimbotSmooth = 5.0f, aimbotDist = 250.0f;
        if (Widgets::FeatureCardStart("aimbot", "Aimbot", &aimbotEnabled, &featureExpanded[0])) {
            Widgets::Slider("##aim_fov", "FOV", &aimbotFov, 1.0f, 180.0f, "%.0f°");
            Widgets::Slider("##aim_smooth", "Smooth", &aimbotSmooth, 0.1f, 30.0f, "%.1f");
            Widgets::Slider("##aim_dist", "Дистанция", &aimbotDist, 10.0f, 1000.0f, "%.0f м");
            Widgets::SettingRow("Head Only"); Widgets::Toggle("##aim_head", &aimbotHeadOnly);
            Widgets::SettingRow("Visible Check"); Widgets::Toggle("##aim_vis", &aimbotVisCheck);
        }
        Widgets::FeatureCardEnd();
    }
    void RenderTheme() {
        ImGui::Text("Presets"); ImGui::Separator();
        for (size_t i = 0; i < ThemeManager::presets.size(); i++) {
            if (ImGui::Button(ThemeManager::presets[i].name.c_str(), ImVec2(120, 30))) { ThemeManager::Apply(ThemeManager::presets[i]); }
            if ((i + 1) % 3 != 0) ImGui::SameLine();
        }
        ImGui::Spacing(); ImGui::Separator(); ImGui::Text("Фон");
        Widgets::SettingRow("Частицы"); Widgets::Toggle("##part_en", &particlesCfg.enabled);
        Widgets::SliderInt("##part_cnt", "Количество", &particlesCfg.count, 10, 150, "%d");
        Widgets::Slider("##part_spd", "Скорость", &particlesCfg.speed, 0.1f, 3.0f, "%.1f×");
        Widgets::Slider("##part_lnk", "Связи", &particlesCfg.linkDistance, 0.0f, 250.0f, "%.0f px");
    }
    void Render() {
        if (!visible) return;
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
        ImGui::SetNextWindowSize(ImVec2(700, 550), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("VV.EXE", &visible, flags)) {
            ImVec2 winPos = ImGui::GetWindowPos();
            ImVec2 winSize = ImGui::GetWindowSize();
            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddRectFilled(winPos, ImVec2(winPos.x + winSize.x, winPos.y + 40), ImGui::ColorConvertFloat4ToU32(ThemeManager::current.sidebarBg));
            dl->AddText(ImVec2(winPos.x + 15, winPos.y + 12), ImGui::GetColorU32(ImGuiCol_Text), "VV.EXE");
            ImGui::SetCursorPos(ImVec2(0, 0));
            ImGui::InvisibleButton("##drag", ImVec2(winSize.x - 60, 40));
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                ImVec2 drag = ImGui::GetMouseDragDelta();
                ImGui::SetWindowPos(ImVec2(winPos.x + drag.x, winPos.y + drag.y));
                ImGui::ResetMouseDragDelta();
            }
            Particles::Update(particlesCfg, ImGui::GetIO().DeltaTime, ImGui::GetMousePos());
            Particles::Render(dl, ImVec2(winPos.x + 140, winPos.y + 40), ImVec2(winSize.x - 140, winSize.y - 40), ThemeManager::GetAccentColor());
            ImGui::SetCursorPos(ImVec2(0, 40));
            RenderSidebar();
            ImGui::SameLine();
            ImGui::BeginChild("##content", ImVec2(0, 0), true);
            ImGui::SetCursorPos(ImVec2(15, 15));
            if (currentTab == 0) RenderCombat();
            else if (currentTab == 7) RenderTheme();
            else ImGui::Text("Tab %d", currentTab);
            ImGui::EndChild();
        }
        ImGui::End();
    }
    void Shutdown() {}
}