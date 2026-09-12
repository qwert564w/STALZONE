#pragma once
#include "../module.h"
namespace Visual {
    class LootEsp : public Module {
    public:
        LootEsp() { name = "loot_esp"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
