#include "particles.h"
#include <cmath>
#include <algorithm>
namespace Particles {
    std::vector<Particle> g_particles;
    void Init(Config& cfg) {
        g_particles.clear();
        for (int i = 0; i < cfg.count; i++) {
            g_particles.push_back({ {rand() % 800, rand() % 600}, {((rand() % 100) / 100.0f - 0.5f) * 0.6f, ((rand() % 100) / 100.0f - 0.5f) * 0.6f}, 0.8f + (rand() % 100) / 100.0f * 1.6f });
        }
    }
    void Update(Config& cfg, float dt, ImVec2 mousePos) {
        if (!cfg.enabled) return;
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();
        if (canvasSize.x <= 0 || canvasSize.y <= 0) return;
        for (auto& p : g_particles) {
            p.pos.x += p.vel.x * cfg.speed;
            p.pos.y += p.vel.y * cfg.speed;
            if (p.pos.x < 0 || p.pos.x > canvasSize.x) p.vel.x *= -1;
            if (p.pos.y < 0 || p.pos.y > canvasSize.y) p.vel.y *= -1;
            p.pos.x = std::clamp(p.pos.x, 0.0f, canvasSize.x);
            p.pos.y = std::clamp(p.pos.y, 0.0f, canvasSize.y);
            ImVec2 d = {mousePos.x - p.pos.x, mousePos.y - p.pos.y};
            float dist = sqrtf(d.x * d.x + d.y * d.y);
            if (dist < cfg.mouseRadius && dist > 0.1f) {
                float force = (cfg.mouseRadius - dist) / cfg.mouseRadius * 0.02f;
                p.vel.x += (d.x / dist) * force;
                p.vel.y += (d.y / dist) * force;
            }
            float sp = sqrtf(p.vel.x * p.vel.x + p.vel.y * p.vel.y);
            if (sp > 1.2f) { p.vel.x = (p.vel.x / sp) * 1.2f; p.vel.y = (p.vel.y / sp) * 1.2f; }
        }
    }
    void Render(ImDrawList* dl, ImVec2 origin, ImVec2 size, ImU32 color) {
        if (g_particles.empty()) return;
        ImVec4 colorF = ImGui::ColorConvertU32ToFloat4(color);
        ImU32 particleColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colorF.x, colorF.y, colorF.z, 0.6f));
        for (size_t i = 0; i < g_particles.size(); i++) {
            auto& p = g_particles[i];
            ImVec2 drawPos = {origin.x + p.pos.x, origin.y + p.pos.y};
            dl->AddCircleFilled(drawPos, p.radius, particleColor);
        }
    }
}