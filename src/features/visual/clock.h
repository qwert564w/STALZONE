#pragma once
#include <imgui.h>
struct ClockConfig { bool enabled = true; float x = 10.0f, y = 10.0f; ImVec4 color = ImVec4(1,1,1,0.8f); bool rainbow = false; float rainbowSpeed = 1.0f; float alpha = 0.8f; bool showDate = false; bool showFPS = false; bool use24h = true; };
namespace Clock { extern ClockConfig cfg; void Render(ImDrawList* dl); void RenderMenu(); }