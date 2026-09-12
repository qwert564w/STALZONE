#pragma once
#include "../module.h"
namespace World {
    class ItemEsp : public Module {
    public:
        ItemEsp() { name = "item_esp"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
