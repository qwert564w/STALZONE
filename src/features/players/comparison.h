#pragma once
#include "../module.h"
namespace Players {
    class Comparison : public Module {
    public:
        Comparison() { name = "comparison"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
