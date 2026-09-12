#pragma once
#include "../module.h"
namespace Visual {
    class Clock : public Module {
    public:
        Clock() { name = "clock"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
