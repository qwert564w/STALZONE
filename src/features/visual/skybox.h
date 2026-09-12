#pragma once
struct SkyboxConfig { bool enabled = false; float color[3] = {0.1f, 0.1f, 0.3f}; float transitionSpeed = 1.0f; bool stars = true; bool aurora = false; float auroraColor[3] = {0.2f, 0.8f, 0.4f}; };
namespace Skybox { extern SkyboxConfig cfg; void Update(); void RenderMenu(); }