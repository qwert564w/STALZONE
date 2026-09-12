#pragma once
#include "../module.h"
namespace Misc {
    class QuickAuction : public Module {
    public:
        QuickAuction() { name = "quick_auction"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
