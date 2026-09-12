// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "aimbot.h"
#include "../../gui/widgets.h"
#include "../../math/math_utils.h"
#include "../../core/offsets.h"
#include <windows.h>
#include <algorithm>
#include <random>
#include <cmath>

static std::mt19937 rng(std::random_device{}());

namespace Combat {

    bool Aimbot::IsAiming() {
        // Read from LocalPlayer component via IL2CPP or memory
        // return Core::IL2CPP::LocalPlayer::GetProperty<bool>("IsAiming");
        return false; // Stub
    }

    bool Aimbot::IsFiring() {
        // return Core::IL2CPP::LocalPlayer::GetProperty<bool>("IsFiring");
        return false; // Stub
    }

    bool Aimbot::CheckVisibilityIL2CPP(Core::IL2CPP::Object* src, Core::IL2CPP::Object* dst) {
        if (!s_visibilityCheck) return true;
        // Use IL2CPP Physics.Raycast to check line of sight
        // This is much safer and more accurate than manual memory raycasting
        // return Core::IL2CPP::Physics::Raycast(src->GetPosition(), dst->GetPosition() - src->GetPosition());
        return true; // Stub
    }

    Vec3 Aimbot::PredictPosition(Vec3 targetPos, Vec3 targetVel, float distance) {
        if (!s_prediction) return targetPos;
        float travelTime = distance / s_bulletSpeed;
        // Basic kinematic prediction
        targetPos.x += targetVel.x * travelTime;
        targetPos.y += targetVel.y * travelTime;
        targetPos.z += targetVel.z * travelTime;
        // Add gravity compensation if targeting head/body over long distances
        targetPos.y += (0.5f * 9.81f * travelTime * travelTime) * 0.5f; 
        return targetPos;
    }

    Vec3 Aimbot::GetBonePosition(Core::IL2CPP::Object* player, TargetBone bone) {
        // In a real IL2CPP environment, we would call Transform::get_position() on the specific bone transform
        // For now, we simulate offset adjustments
        Vec3 basePos = player->GetPosition();
        switch (bone) {
            case TargetBone::HEAD: return {basePos.x, basePos.y + 1.8f, basePos.z};
            case TargetBone::NECK: return {basePos.x, basePos.y + 1.6f, basePos.z};
            case TargetBone::CHEST: return {basePos.x, basePos.y + 1.2f, basePos.z};
            case TargetBone::PELVIS: return {basePos.x, basePos.y + 0.9f, basePos.z};
        }
        return basePos;
    }

    void Aimbot::SmoothAim(Vec3& currentAngle, Vec3 targetAngle) {
        // Calculate delta
        Vec3 delta = targetAngle - currentAngle;
        // Normalize angles
        Math::NormalizeAngles(delta);
        
        // Apply smoothing (Linear Interpolation)
        float smoothFactor = s_smooth > 0.0f ? 1.0f / s_smooth : 1.0f;
        currentAngle.x += delta.x * smoothFactor;
        currentAngle.y += delta.y * smoothFactor;
        currentAngle.z = 0.0f; // No roll
        
        Math::NormalizeAngles(currentAngle);
    }

    void Aimbot::HumanizeMovement() {
        if (!s_humanize) return;
        // Add micro-jitter to simulate human hand tremors and imperfect tracking
        std::uniform_real_distribution<float> jitter(-0.05f, 0.05f);
        // Apply to mouse input directly via SendInput or SetCursorPos in the main loop
        // This prevents server-side snap detection
    }

    void Aimbot::OnUpdate() {
        if (!s_enabled) return;

        // Activation conditions
        if (!s_aimAlways) {
            if (s_aimOnADS && !IsAiming()) return;
            if (s_aimOnFire && !IsFiring()) return;
        }

        if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000) && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
            m_hasTarget = false;
            return;
        }

        // Get Local Player data
        // auto* localPlayer = Core::IL2CPP::LocalPlayer::Get();
        // if (!localPlayer || localPlayer->IsDead()) return;
        
        // Vec3 myPos = localPlayer->GetPosition();
        // Vec3 myVel = localPlayer->GetVelocity();
        // Vec3 currentViewAngles = Core::IL2CPP::Camera::GetViewAngles();
        
        // Simulated target search
        Core::IL2CPP::Object* bestTarget = nullptr;
        float bestScore = 999999.0f;
        
        /*
        auto& entities = Core::IL2CPP::GetEntities("Player");
        for (auto& ent : entities) {
            if (ent->IsDead() || ent->TeamID() == localPlayer->TeamID()) continue;
            if (!CheckVisibilityIL2CPP(localPlayer, ent)) continue;

            Vec3 bonePos = GetBonePosition(ent, s_bone);
            Vec3 relPos = bonePos - myPos;
            float dist = relPos.Length();
            
            Vec3 predictedPos = PredictPosition(bonePos, ent->GetVelocity(), dist);
            Vec3 targetAngle = Math::CalcAngle(myPos, predictedPos);
            
            float fovScore = Math::GetFov(currentViewAngles, targetAngle);
            
            float score = 0.0f;
            if (s_sort == TargetSort::FOV) score = fovScore;
            else if (s_sort == TargetSort::DISTANCE) score = dist;
            else if (s_sort == TargetSort::HEALTH) score = ent->GetHealth();

            if (fovScore > s_fov) continue; // Out of FOV circle
            if (score < bestScore) {
                bestScore = score;
                bestTarget = ent;
            }
        }
        */

        if (bestTarget) {
            if (!m_hasTarget) {
                m_targetAcquiredTime = std::chrono::steady_clock::now();
                m_hasTarget = true;
            }
            
            // Reaction delay simulation
            auto now = std::chrono::steady_clock::now();
            float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_targetAcquiredTime).count() / 1000.0f;
            if (elapsed < s_reactionDelay) return;

            /*
            Vec3 bonePos = GetBonePosition(bestTarget, s_bone);
            Vec3 predictedPos = PredictPosition(bonePos, bestTarget->GetVelocity(), (bonePos - myPos).Length());
            Vec3 targetAngle = Math::CalcAngle(myPos, predictedPos);
            
            SmoothAim(currentViewAngles, targetAngle);
            HumanizeMovement();
            
            // Apply angles
            // Core::IL2CPP::Camera::SetViewAngles(currentViewAngles);
            */
        } else {
            m_hasTarget = false;
        }
    }

    void Aimbot::OnRenderMenu() {
        Widgets::Toggle("Enable", &s_enabled);
        Widgets::Toggle("Visibility Check", &s_visibilityCheck);
        Widgets::Toggle("Prediction", &s_prediction);
        Widgets::Toggle("Humanize", &s_humanize);
        Widgets::Toggle("Aim on ADS", &s_aimOnADS);
        Widgets::Toggle("Aim on Fire", &s_aimOnFire);
        
        Widgets::Slider("FOV", &s_fov, 1.0f, 180.0f);
        Widgets::Slider("Smoothness", &s_smooth, 1.0f, 20.0f);
        Widgets::Slider("Reaction Time (s)", &s_reactionDelay, 0.0f, 0.5f);
        Widgets::Slider("Bullet Speed", &s_bulletSpeed, 50.0f, 1500.0f);
        
        const char* bones[] = { "Head", "Neck", "Chest", "Pelvis" };
        int currentBone = (int)s_bone;
        if (ImGui::Combo("Target Bone", &currentBone, bones, IM_ARRAYSIZE(bones))) {
            s_bone = (TargetBone)currentBone;
        }

        const char* sorts[] = { "FOV", "Distance", "Health" };
        int currentSort = (int)s_sort;
        if (ImGui::Combo("Sort By", &currentSort, sorts, IM_ARRAYSIZE(sorts))) {
            s_sort = (TargetSort)currentSort;
        }
    }

    void Aimbot::OnRenderOverlay() {
        if (!s_enabled) return;
        ImDrawList* dl = ImGui::GetBackgroundDrawList();
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        
        // Draw FOV Circle
        dl->AddCircle(ImVec2(w/2.0f, h/2.0f), s_fov * (w/90.0f), IM_COL32(255, 255, 255, 150), 128, 1.0f);
        
        if (m_hasTarget) {
            // Draw lock-on indicator
            dl->AddRectFilled(ImVec2(w/2.0f - 5, h/2.0f - 5), ImVec2(w/2.0f + 5, h/2.0f + 5), IM_COL32(255, 0, 0, 200));
        }
    }
}
