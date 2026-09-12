// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <cstdint>

namespace VV {
namespace Combat {

class NoRecoil {
public:
    static bool s_enabled;
    static float s_strength;
    static bool s_verticalOnly;
    static bool s_horizontalOnly;
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
    
private:
    static void CompensateRecoil();
    static float s_lastPunchAngleX;
    static float s_lastPunchAngleY;
};

} // namespace Combat
} // namespace VV
