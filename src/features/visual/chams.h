// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once

namespace VV {
namespace Visual {

class Chams {
public:
    static bool s_enabled;
    static bool s_visibleOnly;
    static float s_colorEnemy[4];
    static float s_colorAlly[4];
    static float s_colorVisible[4];
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
    static void ApplyChams(void* player, void* material);
};

} // namespace Visual
} // namespace VV
