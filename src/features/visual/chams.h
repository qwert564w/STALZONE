#pragma once
#include "../module.h"
namespace Visual {
    class Chams : public Module {
    public:
        Chams() { name = "chams"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
