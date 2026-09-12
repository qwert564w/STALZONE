#pragma once
struct PanicKeyConfig { bool enabled = true; int key = VK_END; };
namespace PanicKey { extern PanicKeyConfig cfg; void Update(); void RenderMenu(); }