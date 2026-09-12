#pragma once
#include <imgui.h>
struct AimbotConfig { bool enabled = false; int key = VK_XBUTTON2; float fov = 12.0f; float smooth = 0.25f; float maxDistance = 200.0f; bool headOnly = false; bool visibleCheck = true; bool ignoreTeam = true; bool humanize = true; float reactionDelay = 0.12f; bool randomMiss = true; float missChance = 0.08f; };
namespace Aimbot { extern AimbotConfig cfg; void Update(); void RenderMenu(); void Shutdown(); }