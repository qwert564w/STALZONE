#pragma once
#include <imgui.h>
struct SilentAimConfig { bool enabled = false; float fov = 25.0f; float maxDistance = 250.0f; bool randomMiss = true; float missChance = 0.15f; bool onlyVisible = true; bool headOnly = false; bool showHitbox = true; };
namespace SilentAim { extern SilentAimConfig cfg; void Update(); void RenderMenu(); }