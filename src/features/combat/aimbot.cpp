#include "aimbot.h"
#include "../../gui/widgets.h"
namespace Combat {
    void Aimbot::OnUpdate() { /* Find entities, check distance/fov, calc angles, smooth lerp */ }
    void Aimbot::OnRenderMenu() { Widgets::Slider(&fov, 0, 180); }
    void Aimbot::OnRenderOverlay() {  }
}
