#pragma once
#include "../module.h"
namespace Combat {
    class FovCircle : public Module {
    public:
        FovCircle() { name = "fov_circle"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
