// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once

namespace VV {
namespace Visual {

class LootESP {
public:
    static bool s_enabled;
    static int s_minRarity;
    static float s_maxDistance;
    static bool s_showIcons;
    static bool s_showNames;
    static float s_colorCommon[4];
    static float s_colorRare[4];
    static float s_colorEpic[4];
    static float s_colorLegendary[4];
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
};

} // namespace Visual
} // namespace VV
