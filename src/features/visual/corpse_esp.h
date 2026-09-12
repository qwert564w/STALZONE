#pragma once
#include <imgui.h>
struct CorpseESPConfig { bool enabled = false; float skullColor[4] = {0.9f, 0.9f, 0.9f, 0.9f}; bool showLoot = true; bool showName = true; };
namespace CorpseESP { extern CorpseESPConfig cfg; void Render(ImDrawList* dl, int w, int h); void RenderMenu(); }