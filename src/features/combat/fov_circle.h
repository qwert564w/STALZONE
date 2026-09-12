// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once

namespace VV {
namespace Combat {

class FovCircle {
public:
    static bool s_enabled;
    static float s_fov;
    static float s_color[4]; // RGBA
    static float s_thickness;
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
};

} // namespace Combat
} // namespace VV
