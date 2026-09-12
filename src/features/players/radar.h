#pragma once
struct RadarConfig { bool enabled = false; float radius = 150.0f; bool showNames = false; };
namespace Radar { extern RadarConfig cfg; void Render(ImDrawList* dl); void RenderMenu(); }