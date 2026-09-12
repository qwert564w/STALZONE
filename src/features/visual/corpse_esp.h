// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once

namespace VV {
namespace Visual {

class CorpseESP {
public:
    static bool s_enabled;
    static bool s_showLoot;
    static bool s_showSkull;
    static float s_color[4];
    static float s_maxDistance;
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
};

} // namespace Visual
} // namespace VV
