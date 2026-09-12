#pragma once
#include "../module.h"
namespace Combat {
    class SilentAim : public Module {
    public:
        SilentAim() { name = "silent_aim"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
