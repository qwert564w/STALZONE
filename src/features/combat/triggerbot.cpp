#include "triggerbot.h"
#include "../../gui/widgets.h"
namespace Combat {
    void Triggerbot::OnUpdate() {
        /* Check crosshair target, sleep, mouse_event click */
    }
    void Triggerbot::OnRenderMenu() {
        Widgets::Slider(&delay, 0, 500);
    }
    void Triggerbot::OnRenderOverlay() {
        
    }
}
