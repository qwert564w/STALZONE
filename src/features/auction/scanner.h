#pragma once
#include "../module.h"
namespace Auction {
    class Scanner : public Module {
    public:
        Scanner() { name = "scanner"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
