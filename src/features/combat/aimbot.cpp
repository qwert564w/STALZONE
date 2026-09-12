#include "aimbot.h"
#include "../../gui/widgets.h"
#include "../../math/math_utils.h"
#include "../../core/offsets.h"
#include <windows.h>
#include <algorithm>
#include <random>
#include <chrono>

static std::mt19937 rng(std::random_device{}());

bool Aimbot::IsAiming() { return false; }
bool Aimbot::IsFiring() { return false; }
bool Aimbot::CheckVisibility(Vec3 src, Vec3 dst) { return true; }

void Aimbot::OnUpdate() {
    if (!enabled) return;
    if (!aimAlways) {
        if (aimOnADS && !IsAiming()) return;
        if (aimOnFire && !IsFiring()) return;
    }

    // Pseudo-code for targeting logic:
    // Vec3 myPos = Read<Vec3>(LocalPlayer + Offsets::Player::Position);
    // for each ent {
    //   Vec3 screenPos;
    //   if (!Math::WorldToScreen(ent->Position, screenPos, viewMatrix, W, H)) continue;
    //   float dist = sqrtf(powf(screenPos.x - W/2, 2) + powf(screenPos.y - H/2, 2));
    //   if (dist > fov) continue;
    //   if (!CheckVisibility(myPos, ent->Position)) continue;
    //   
    //   float r = std::uniform_real_distribution<float>(0.0f, 1.0f)(rng);
    //   Vec3 target = ent->Position;
    //   if (r < headChance) target.y += 1.8f;
    //   else if (r < headChance + chestChance) target.y += 1.2f;
    //   
    //   if (std::uniform_real_distribution<float>(0, 1)(rng) < missChance) {
    //       target.x += std::uniform_real_distribution<float>(-0.3f, 0.3f)(rng);
    //   }
    //   Vec3 calcAngle = Math::CalcAngle(myPos, target);
    //   SmoothAim(calcAngle, smooth);
    // }
}

void Aimbot::OnRenderMenu() {
    Widgets::Toggle("Always", &aimAlways);
    Widgets::Toggle("On ADS", &aimOnADS);
    Widgets::Toggle("On Fire", &aimOnFire);
    Widgets::Slider("FOV", &fov, 1.0f, 180.0f);
    Widgets::Slider("Smooth", &smooth, 1.0f, 20.0f);
    Widgets::Slider("Head %", &headChance, 0.0f, 1.0f);
    Widgets::Slider("Chest %", &chestChance, 0.0f, 1.0f);
    Widgets::Slider("Body %", &bodyChance, 0.0f, 1.0f);
    Widgets::Slider("Miss %", &missChance, 0.0f, 1.0f);
    Widgets::Slider("Reaction", &reactionDelay, 0.0f, 1.0f);
}

void Aimbot::OnRenderOverlay() {
    if (!enabled) return;
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    dl->AddCircle(ImVec2(w/2.0f, h/2.0f), fov, IM_COL32(255, 255, 255, 150), 64, 1.0f);
}
