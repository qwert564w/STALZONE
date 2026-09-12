#pragma once
struct NoSpreadConfig { bool enabled = false; bool scopeOnly = true; };
namespace NoSpread { extern NoSpreadConfig cfg; void Update(); void RenderMenu(); }