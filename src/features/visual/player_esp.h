#pragma once
#include <imgui.h>
struct PlayerESPConfig { bool enabled = false; bool boxes = true; bool health = true; bool names = false; bool skeleton = true; bool teamCheck = true; bool distance = true; bool weapon = true; bool botPlayer = true; bool winFalse = true; float maxDistance = 300.0f; ImVec4 color = ImVec4(0.28f, 0.55f, 1.0f, 0.85f); };
namespace PlayerESP { extern PlayerESPConfig cfg; void Render(ImDrawList* dl, int w, int h); void RenderMenu(); }