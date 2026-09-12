#include "menu.h"
#include "imgui.h"
#include "../features/registry.h"
#include "theme.h"
#include "widgets.h"
namespace Menu {
    bool isOpen = true;
    void Init() { Theme::Apply(Theme::Dark); }
    void Render() {
        if (!isOpen) return;
        ImGui::Begin("STALZONE v1.0 | Professional", &isOpen, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginChild("Sidebar", ImVec2(150, 0), true);
        const char* tabs[] = {"Combat", "Visual", "Players", "Auction", "World", "Misc"};
        static int active = 0;
        for (int i = 0; i < 6; i++) {
            if (ImGui::Selectable(tabs[i], active == i)) active = i;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("Content");
        Features::Registry::RenderTab(active);
        ImGui::EndChild();
        ImGui::End();
    }
}
