#include "silent_aim.h"
#include "../../gui/widgets.h"
namespace Combat {
    void SilentAim::OnUpdate() { /* Hook internal shoot function, override raycast dir */ }
    void SilentAim::OnRenderMenu() {  }
    void SilentAim::OnRenderOverlay() {  }
}
