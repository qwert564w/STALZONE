#pragma once
#include "../module.h"
namespace Players {
    class Radar : public Module {
    public:
        Radar() { name = "radar"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
