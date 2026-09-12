#pragma once
struct ComparisonConfig { bool enabled = true; bool showPowerDiff = true; bool showEquipmentIcons = true; float updateInterval = 0.5f; };
struct PlayerPower { int armorTier = 0; int helmetTier = 0; int weaponTier = 0; int artifactCount = 0; float total = 0.0f; };
namespace Comparison { extern ComparisonConfig cfg; float CalculatePower(PlayerPower& p); const char* GetWinLose(float localPower, float targetPower); void RenderMenu(); }