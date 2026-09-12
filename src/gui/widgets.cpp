#include "widgets.h"
#include "theme.h"
#include <imgui_internal.h>
#include <cstdio>
namespace Widgets {
    bool Toggle(const char* id, bool* value) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        const float height = 20.0f, width = 36.0f;
        const ImVec2 pos = window->DC.CursorPos;
        const ImGuiID id_hash = window->GetID(id);
        ImDrawList* dl = window->DrawList;
        Theme theme = ThemeManager::current;
        ImU32 bg_col = *value ? ImGui::ColorConvertFloat4ToU32(theme.accent) : ImGui::GetColorU32(ImGuiCol_FrameBg);
        ImU32 border_col = *value ? ImGui::ColorConvertFloat4ToU32(theme.accent) : ImGui::GetColorU32(ImGuiCol_Border);
        dl->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), bg_col, height * 0.5f);
        dl->AddRect(pos, ImVec2(pos.x + width, pos.y + height), border_col, height * 0.5f);
        const float circle_pad = 2.0f, circle_size = height - circle_pad * 2.0f;
        const float circle_x = *value ? (pos.x + width - circle_size - circle_pad) : (pos.x + circle_pad);
        const float circle_y = pos.y + circle_pad;
        ImU32 circle_col = *value ? IM_COL32(255, 255, 255, 255) : ImGui::GetColorU32(ImGuiCol_TextDisabled);
        dl->AddCircleFilled(ImVec2(circle_x + circle_size * 0.5f, circle_y + circle_size * 0.5f), circle_size * 0.5f, circle_col);
        bool hovered = false, held = false;
        bool pressed = ImGui::ButtonBehavior(ImRect(pos, ImVec2(pos.x + width, pos.y + height)), id_hash, &hovered, &held);
        if (pressed) *value = !*value;
        ImGui::Dummy(ImVec2(width, height));
        return pressed;
    }
    bool FeatureCardStart(const char* id, const char* name, bool* enabled, bool* expanded) {
        ImGui::PushID(id);
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGui::BeginChild(ImGui::GetID("card"), ImVec2(0, *expanded ? 0 : 42.0f), true, ImGuiWindowFlags_NoScrollbar);
        ImVec2 card_pos = ImGui::GetWindowPos();
        ImVec2 card_size = ImGui::GetWindowSize();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        Theme theme = ThemeManager::current;
        if (*enabled) {
            dl->AddRectFilled(card_pos, ImVec2(card_pos.x + card_size.x, card_pos.y + card_size.y), IM_COL32(26, 26, 26, 255), 8.0f);
            dl->AddRect(card_pos, ImVec2(card_pos.x + card_size.x, card_pos.y + card_size.y), ImGui::ColorConvertFloat4ToU32(theme.accent), 8.0f);
        } else {
            dl->AddRect(card_pos, ImVec2(card_pos.x + card_size.x, card_pos.y + card_size.y), ImGui::GetColorU32(ImGuiCol_Border), 8.0f);
        }
        ImGui::SetCursorPos(ImVec2(12, 10));
        if (ImGui::InvisibleButton("gear", ImVec2(22, 22))) { *expanded = !*expanded; }
        if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        ImVec2 gear_center = ImVec2(card_pos.x + 23, card_pos.y + 21);
        dl->AddCircle(gear_center, 4.0f, ImGui::GetColorU32(ImGuiCol_TextDisabled), 8, 1.5f);
        dl->AddLine(ImVec2(gear_center.x - 3, gear_center.y), ImVec2(gear_center.x + 3, gear_center.y), ImGui::GetColorU32(ImGuiCol_TextDisabled), 1.5f);
        if (*expanded) dl->AddLine(ImVec2(gear_center.x, gear_center.y - 3), ImVec2(gear_center.x, gear_center.y + 3), ImGui::GetColorU32(ImGuiCol_TextDisabled), 1.5f);
        ImGui::SameLine();
        ImGui::SetCursorPosX(42);
        ImGui::SetCursorPosY(12);
        ImU32 text_col = *enabled ? IM_COL32(255, 255, 255, 255) : ImGui::GetColorU32(ImGuiCol_Text);
        ImGui::PushStyleColor(ImGuiCol_Text, text_col);
        ImGui::Text("%s", name);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::SetCursorPosX(card_size.x - 50);
        ImGui::SetCursorPosY(11);
        Toggle("##toggle", enabled);
        if (*expanded) {
            ImGui::SetCursorPosY(46);
            ImGui::Indent(12);
            ImGui::BeginChild("content", ImVec2(0, 0), false);
        }
        return *expanded;
    }
    void FeatureCardEnd() {
        if (ImGui::GetCurrentWindow()->DC.CursorPos.y > 46) { ImGui::EndChild(); ImGui::Unindent(12); }
        ImGui::EndChild();
        ImGui::PopID();
        ImGui::Spacing();
    }
    bool SettingRow(const char* label) {
        ImGui::PushID(label);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::SameLine(ImGui::GetWindowWidth() - 60);
        ImGui::PopID();
        return true;
    }
    bool Slider(const char* id, const char* label, float* value, float min, float max, const char* fmt) {
        ImGui::PushID(id);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::SameLine(ImGui::GetWindowWidth() - 60);
        char val_buf[32]; snprintf(val_buf, sizeof(val_buf), fmt, *value);
        ImGui::PushStyleColor(ImGuiCol_Text, ThemeManager::current.accentBright);
        ImGui::Text("%s", val_buf);
        ImGui::PopStyleColor();
        bool changed = ImGui::SliderFloat("##slider", value, min, max, "");
        ImGui::PopID();
        return changed;
    }
    bool SliderInt(const char* id, const char* label, int* value, int min, int max, const char* fmt) {
        ImGui::PushID(id);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::SameLine(ImGui::GetWindowWidth() - 60);
        char val_buf[32]; snprintf(val_buf, sizeof(val_buf), fmt, *value);
        ImGui::PushStyleColor(ImGuiCol_Text, ThemeManager::current.accentBright);
        ImGui::Text("%s", val_buf);
        ImGui::PopStyleColor();
        bool changed = ImGui::SliderInt("##slider", value, min, max, "");
        ImGui::PopID();
        return changed;
    }
    void SectionGroup(const char* label) {
        ImGui::Spacing();
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
        ImGui::Text("%s", label);
        ImGui::PopStyleColor();
        ImGui::Separator();
        ImGui::Spacing();
    }
}