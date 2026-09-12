// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <cstdint>
#include <chrono>

namespace VV {
namespace Combat {

class Triggerbot {
public:
    static bool s_enabled;
    static int s_delayMin;
    static int s_delayMax;
    static float s_missChance;
    static bool s_headOnly;
    static bool s_visibilityCheck;
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
    
private:
    static bool IsCrosshairOnEnemy();
    static bool IsVisible(void* player);
    static void PerformClick();
    static std::chrono::steady_clock::time_point s_lastClick;
};

} // namespace Combat
} // namespace VV
