#pragma once
#include "../module.h"
namespace Combat {
    class NoSpread : public Module {
    public:
        NoSpread() { name = "no_spread"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
