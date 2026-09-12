#include "widgets.h"
#include "imgui.h"
#include <windows.h>
namespace Widgets {
    bool Toggle(const char* label, bool* v) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, *v ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        bool changed = ImGui::Checkbox(label, v);
        ImGui::PopStyleColor();
        return changed;
    }
    bool Slider(const char* label, float* v, float min, float max) { return ImGui::SliderFloat(label, v, min, max); }
    bool Keybind(const char* label, int* key) {
        ImGui::PushID(label);
        if (ImGui::Button(label)) { }
        ImGui::SameLine(); ImGui::Text("%d", *key);
        ImGui::PopID(); return false;
    }
    void FeatureCard(const char* name, bool enabled) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
        ImGui::BeginChild(name, ImVec2(150, 80), true);
        ImGui::Text("%s", name);
        ImGui::TextColored(enabled ? ImVec4(0,1,0,1) : ImVec4(1,0,0,1), enabled ? "ACTIVE" : "INACTIVE");
        ImGui::EndChild(); ImGui::PopStyleVar();
    }
}
