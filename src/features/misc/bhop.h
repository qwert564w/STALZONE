#pragma once
#include "../module.h"
namespace Misc {
    class Bhop : public Module {
    public:
        Bhop() { name = "bhop"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
