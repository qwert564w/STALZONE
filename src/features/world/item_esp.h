#pragma once
struct ItemESPConfig { bool enabled = false; bool weapons = true; bool ammo = true; bool medkits = false; bool artifacts = false; float maxDistance = 300.0f; };
namespace ItemESP { extern ItemESPConfig cfg; void Render(); void RenderMenu(); }