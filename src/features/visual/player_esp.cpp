#include "player_esp.h"
#include "../../gui/widgets.h"
#include "../../math/math_utils.h"
namespace Visual {
    bool show_boxes = true;
    void PlayerEsp::OnUpdate() { }
    void PlayerEsp::OnRenderMenu() { Widgets::Toggle("Show Boxes", &show_boxes); }
    void PlayerEsp::OnRenderOverlay() {
        // 1. Get ViewMatrix
        // 2. For each player:
        // Vec3 screenPos;
        // if (Math::WorldToScreen(playerPos, screenPos, viewMatrix, screenWidth, screenHeight)) {
        //     Draw 2D Box around screenPos
        // }
    }
}
