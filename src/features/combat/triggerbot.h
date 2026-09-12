#pragma once
#include "../module.h"
namespace Combat {
    class Triggerbot : public Module {
    public:
        Triggerbot() { name = "triggerbot"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
