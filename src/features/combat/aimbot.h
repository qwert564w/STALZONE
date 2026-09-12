#pragma once
#include "../module.h"
namespace Combat {
    class Aimbot : public Module {
    public:
        Aimbot() { name = "aimbot"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
