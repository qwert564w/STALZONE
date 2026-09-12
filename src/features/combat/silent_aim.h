// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <cstdint>
#include <cmath>
#include <random>
#include <chrono>

namespace VV {
namespace Combat {

class SilentAim_H {
public:
    static bool s_enabled;
    static float s_fov;
    static bool s_visibilityCheck;
    static bool s_prediction;
    static float s_smoothness;
    static bool s_jitter;
    static float s_jitterAmount;
    
    struct Target {
        void* player;
        uint64_t boneIndex;
        float distance;
        bool visible;
    };
    
    static Target s_currentTarget;
    
    static void OnUpdate();
    static void OnRenderMenu();
    static void OnRenderOverlay();
    
private:
    static bool IsVisible(void* player, uint64_t boneIndex);
    static void PredictPosition(void* player, float* outPos);
    static void ApplyJitter(float& x, float& y, float& z);
    static Target FindBestTarget();
    static void OverrideShootAngle(float* angle, float targetX, float targetY, float targetZ);
};

} // namespace Combat
} // namespace VV
