#include "clock.h"
#include "../../gui/widgets.h"
namespace Visual {
    void Clock::OnUpdate() {
        
    }
    void Clock::OnRenderMenu() {
        Widgets::Toggle(&rainbow, &rainbow);
    }
    void Clock::OnRenderOverlay() {
        /* Draw time, date, FPS */
    }
}
