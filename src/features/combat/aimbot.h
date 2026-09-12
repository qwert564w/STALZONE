// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include "../module.h"
#include "../../math/math_utils.h"
#include "../../core/il2cpp_bridge.h"
#include <vector>
#include <chrono>

namespace Combat {
    enum class TargetBone { HEAD, NECK, CHEST, PELVIS };
    enum class TargetSort { FOV, DISTANCE, HEALTH };

    class Aimbot : public Module {
    public:
        Aimbot() { name = "Advanced Aimbot"; }
        void OnUpdate() override;
        void OnRenderMenu() override;
        void OnRenderOverlay() override;

        // Settings
        bool s_enabled = false;
        bool s_aimOnADS = false;
        bool s_aimOnFire = false;
        bool s_visibilityCheck = true;
        bool s_prediction = true;
        bool s_humanize = true;
        
        float s_fov = 15.0f;
        float s_smooth = 5.0f;
        float s_reactionDelay = 0.05f;
        float s_bulletSpeed = 300.0f;
        
        TargetBone s_bone = TargetBone::HEAD;
        TargetSort s_sort = TargetSort::FOV;

    private:
        bool IsAiming();
        bool IsFiring();
        bool CheckVisibilityIL2CPP(Core::IL2CPP::Object* src, Core::IL2CPP::Object* dst);
        
        Vec3 PredictPosition(Vec3 targetPos, Vec3 targetVel, float distance);
        Vec3 GetBonePosition(Core::IL2CPP::Object* player, TargetBone bone);
        
        void SmoothAim(Vec3& currentAngle, Vec3 targetAngle);
        void HumanizeMovement();

        std::chrono::steady_clock::time_point m_lastAimTime;
        std::chrono::steady_clock::time_point m_targetAcquiredTime;
        bool m_hasTarget = false;
    };
}
