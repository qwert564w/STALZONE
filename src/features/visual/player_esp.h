// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once

namespace VV {
namespace Visual {

class PlayerESP {
public:
    static bool s_enabled;
    static bool s_showBoxes;
    static bool s_showNames;
    static bool s_showHealth;
    static bool s_showDistance;
    static bool s_showBones;
    static float s_colorEnemy[4];
    static float s_colorAlly[4];
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
};

} // namespace Visual
} // namespace VV
