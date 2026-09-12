#pragma once
struct BhopConfig { bool enabled = false; int chance = 100; };
namespace Bhop { extern BhopConfig cfg; void Update(); void RenderMenu(); }