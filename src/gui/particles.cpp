#include "particles.h"
#include "imgui.h"
#include <cmath>
std::vector<Particle> pts;
namespace Particles {
    void Update() {
        if (pts.empty()) { for (int i = 0; i < 50; i++) pts.push_back({(float)rand()%800, (float)rand()%600, 0.5f, 0.5f}); }
        for (auto& p : pts) { p.x += p.vx; p.y += p.vy;
            if (p.x < 0 || p.x > 800) p.vx = -p.vx;
            if (p.y < 0 || p.y > 600) p.vy = -p.vy;
        }
    }
    void Render() {
        ImDrawList* dl = ImGui::GetBackgroundDrawList();
        for (auto& p : pts) { dl->AddCircleFilled(ImVec2(p.x, p.y), 2.0f, IM_COL32(255,255,255,100)); }
    }
}
