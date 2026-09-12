#pragma once
#include <vector>
#include <imgui.h>
struct Particle { float x, y, vx, vy, r; };
struct ParticlesConfig { bool enabled = true; int count = 40; float speed = 1.0f; float links = 120.0f; float mouseRadius = 140.0f; };
namespace Particles { void Init(ParticlesConfig& cfg); void Update(ParticlesConfig& cfg, float dt); void Render(ImDrawList* dl, ImVec2 origin, ImVec2 size, ImVec4 accentColor); }