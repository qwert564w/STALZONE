#pragma once
#include "../module.h"
namespace Combat {
    class NoRecoil : public Module {
    public:
        NoRecoil() { name = "no_recoil"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
