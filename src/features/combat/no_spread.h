// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <cstdint>

namespace VV {
namespace Combat {

class NoSpread {
public:
    static bool s_enabled;
    static float s_reductionPercent;
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
    
private:
    static void ReduceSpread();
};

} // namespace Combat
} // namespace VV
