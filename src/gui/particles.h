#pragma once
#include <imgui.h>
#include <vector>
namespace Particles {
    struct Particle { ImVec2 pos; ImVec2 vel; float radius; };
    struct Config { bool enabled = true; int count = 40; float speed = 1.0f; float linkDistance = 120.0f; float mouseRadius = 140.0f; };
    void Init(Config& cfg);
    void Update(Config& cfg, float dt, ImVec2 mousePos);
    void Render(ImDrawList* dl, ImVec2 origin, ImVec2 size, ImU32 color);
}