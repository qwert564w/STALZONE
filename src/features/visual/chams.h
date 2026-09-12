#pragma once
struct ChamsConfig { bool enabled = false; bool throughWalls = true; bool useTeamColor = true; int style = 0; float enemyColor[4] = {1.0f, 0.2f, 0.2f, 1.0f}; float teamColor[4] = {0.2f, 0.8f, 0.2f, 1.0f}; float alpha = 0.85f; };
namespace Chams { extern ChamsConfig cfg; void Update(); void RenderMenu(); }