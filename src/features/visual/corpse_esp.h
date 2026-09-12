#pragma once
#include "../module.h"
namespace Visual {
    class CorpseEsp : public Module {
    public:
        CorpseEsp() { name = "corpse_esp"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
