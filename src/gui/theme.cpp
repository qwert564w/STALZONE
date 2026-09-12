#include "theme.h"
#include "imgui.h"
namespace Theme {
    void Apply(Preset p) {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;
        if (p == Dark) {
            colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
            colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
        } else if (p == Midnight) {
            colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.08f, 1.0f);
        }
    }
}
