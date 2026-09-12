#pragma once
#include "../module.h"
namespace Visual {
    class Skybox : public Module {
    public:
        Skybox() { name = "skybox"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
