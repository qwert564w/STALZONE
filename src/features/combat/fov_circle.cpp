#include "fov_circle.h"
#include "../../gui/widgets.h"
namespace Combat {
    void FovCircle::OnUpdate() {  }
    void FovCircle::OnRenderMenu() { Widgets::Slider(&fov, 0, 180); }
    void FovCircle::OnRenderOverlay() { /* Draw circle at screen center with radius fov */ }
}
