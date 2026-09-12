// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "menu.h"
#include "widgets.h"
#include "theme.h"
#include "../features/visual/artifact_glow.h"
#include "../features/visual/anomaly_esp.h"
#include "../features/visual/player_esp.h"
#include "../features/combat/aimbot.h"
#include "../bypass/bypass.h"
#include "../config/config.h"
#include <imgui.h>

namespace VV {
    namespace GUI {
        bool Menu::s_isOpen = true;

        void Menu::Init() {
            Theme::ApplyDarkStalkerTheme();
        }

        void Menu::Render() {
            if (!s_isOpen) return;

            ImGui::Begin("VV.EXE | STALZONE [BUILD 2026.09]", &s_isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
            
            if (ImGui::BeginTabBar("##MainTabs")) {
                if (ImGui::BeginTabItem("Combat")) {
                    Widgets::FeatureCardStart("Aimbot");
                    ImGui::Checkbox("Enable", &Combat::Aimbot::s_enabled);
                    ImGui::SliderFloat("FOV", &Combat::Aimbot::s_fov, 1.0f, 180.0f);
                    ImGui::SliderFloat("Smooth", &Combat::Aimbot::s_smooth, 1.0f, 10.0f);
                    Widgets::FeatureCardEnd();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Visual")) {
                    RenderVisualTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Misc")) {
                    RenderMiscTab();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Bypass")) {
                    RenderBypassTab();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
            
            ImGui::End();
        }

        void Menu::RenderVisualTab() {
            Widgets::FeatureCardStart("Player ESP");
            ImGui::Checkbox("Enable", &Visuals::PlayerESP::s_enabled);
            ImGui::SliderFloat("Box Thickness", &Visuals::PlayerESP::s_thickness, 1.0f, 5.0f);
            Widgets::FeatureCardEnd();

            Widgets::FeatureCardStart("Artifact Glow");
            ImGui::Checkbox("Enable", &Visuals::ArtifactGlow::s_enabled);
            ImGui::SliderFloat("Intensity", &Visuals::ArtifactGlow::s_intensity, 0.5f, 5.0f);
            ImGui::SliderFloat("Radius", &Visuals::ArtifactGlow::s_radius, 10.0f, 200.0f);
            ImGui::Checkbox("Rare Only", &Visuals::ArtifactGlow::s_rareOnly);
            Widgets::FeatureCardEnd();

            Widgets::FeatureCardStart("Anomaly ESP");
            ImGui::Checkbox("Enable", &Visuals::AnomalyESP::s_enabled);
            ImGui::SliderFloat("Line Thickness", &Visuals::AnomalyESP::s_thickness, 1.0f, 5.0f);
            ImGui::Checkbox("Show Name", &Visuals::AnomalyESP::s_showName);
            ImGui::Checkbox("Show Damage Radius", &Visuals::AnomalyESP::s_showRadius);
            Widgets::FeatureCardEnd();
        }

        void Menu::RenderMiscTab() {
            ImGui::Text("Misc features will be added here.");
        }

        void Menu::RenderBypassTab() {
            ImGui::TextWrapped("The bypass engine automatically detects your environment and selects the best strategy. You can also force a specific mode below.");
            ImGui::Spacing();
            
            const char* strategies[] = { "AGGRESSIVE", "BALANCED", "STEALTH", "GHOST" };
            int current = (int)Bypass::CoreBypass::Get().GetCurrentStrategy();
            
            if (ImGui::Combo("Force Strategy", &current, strategies, IM_ARRAYSIZE(strategies))) {
                Bypass::CoreBypass::Get().SetStrategy((Bypass::Strategy)current);
            }
            
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Status: ACTIVE & MONITORING");
        }
    }
}
