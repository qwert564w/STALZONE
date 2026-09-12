#pragma once
struct TriggerbotConfig { bool enabled = false; int delayMs = 50; float maxDistance = 100.0f; bool scopeOnly = true; };
namespace Triggerbot { extern TriggerbotConfig cfg; void Update(); void RenderMenu(); }