#pragma once
#include <imgui.h>
enum ItemRarity { RARITY_COMMON = 0, RARITY_UNCOMMON = 1, RARITY_RARE = 2, RARITY_EPIC = 3, RARITY_LEGENDARY = 4, RARITY_MYTHIC = 5 };
struct LootESPConfig { bool enabled = false; bool showWeapons = true; bool showAmmo = true; bool showMedkits = false; bool showArtifacts = true; bool showContainers = true; float maxDistance = 300.0f; };
namespace LootESP { extern LootESPConfig cfg; ImU32 GetRarityColor(ItemRarity r); void Render(ImDrawList* dl, int w, int h); void RenderMenu(); }