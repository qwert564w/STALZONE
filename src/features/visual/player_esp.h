#pragma once
#include "../module.h"
namespace Visual {
    class PlayerEsp : public Module {
    public:
        PlayerEsp() { name = "player_esp"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
