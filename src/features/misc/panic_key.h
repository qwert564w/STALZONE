#pragma once
#include "../module.h"
namespace Misc {
    class PanicKey : public Module {
    public:
        PanicKey() { name = "panic_key"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    };
}
