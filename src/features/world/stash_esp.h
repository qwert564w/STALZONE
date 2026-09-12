#pragma once
struct StashESPConfig { bool enabled = false; bool showDistance = true; };
namespace StashESP { extern StashESPConfig cfg; void Render(); void RenderMenu(); }