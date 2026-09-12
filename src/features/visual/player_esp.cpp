#include "player_esp.h"
#include "../../gui/widgets.h"
namespace Visual {
    void PlayerEsp::OnUpdate() {
        
    }
    void PlayerEsp::OnRenderMenu() {
        Widgets::Toggle(&show_boxes, &show_boxes);
    }
    void PlayerEsp::OnRenderOverlay() {
        /* World2Screen, draw 2D box, health, name */
    }
}
