#pragma once
#include <imgui.h>
struct FovCircleConfig { bool enabled = false; float radius = 150.0f; ImVec4 color = ImVec4(1,1,1,0.6f); bool showCrosshair = true; bool showFovText = true; };
namespace FovCircle { extern FovCircleConfig cfg; void RenderOverlay(ImDrawList* dl); void RenderMenu(); }