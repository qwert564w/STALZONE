#pragma once
struct NoRecoilConfig { bool enabled = false; float compensation = 0.72f; float randomization = 0.08f; bool onlyVertical = false; };
namespace NoRecoil { extern NoRecoilConfig cfg; void Update(); void RenderMenu(); }