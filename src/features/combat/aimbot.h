#pragma once
#include "../module.h"
#include "../../math/math_utils.h"

namespace Combat {
    class Aimbot : public Module {
    public:
        float fov = 15.0f;
        float smooth = 5.0f;
        float reactionDelay = 0.05f;
        float missChance = 0.02f;
        float headChance = 0.70f;
        float chestChance = 0.25f;
        float bodyChance = 0.05f;
        bool aimOnADS = false;
        bool aimOnFire = false;
        bool aimAlways = true;
        
        Aimbot() { name = "Aimbot"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;
    private:
        bool CheckVisibility(Vec3 src, Vec3 dst);
        bool IsAiming();
        bool IsFiring();
    };
}
