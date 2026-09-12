#pragma once
#include "../module.h"
namespace World {
    class StashEsp : public Module {
    public:
        StashEsp() { name = "stash_esp"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
