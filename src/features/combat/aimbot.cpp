#include "aimbot.h"
#include "../../gui/widgets.h"
#include "../../math/math_utils.h"
namespace Combat {
    float fov = 10.0f;
    void Aimbot::OnUpdate() {
        // 1. Get LocalPlayer and EntityList via IL2CPP Bridge
        // 2. Find closest enemy within FOV
        // 3. Vec3 targetPos = ...; Vec3 myPos = ...;
        // 4. Vec3 angle = Math::CalcAngle(myPos, targetPos);
        // 5. Apply smooth lerp to view angles
    }
    void Aimbot::OnRenderMenu() { Widgets::Slider("FOV", &fov, 0, 180); }
    void Aimbot::OnRenderOverlay() { }
}
