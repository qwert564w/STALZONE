// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "triggerbot.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include <windows.h>
#include <random>
#include <thread>

namespace VV {
namespace Combat {

bool Triggerbot::s_enabled = false;
int Triggerbot::s_delayMin = 50;
int Triggerbot::s_delayMax = 200;
float Triggerbot::s_missChance = 5.0f;
bool Triggerbot::s_headOnly = false;
bool Triggerbot::s_visibilityCheck = true;
std::chrono::steady_clock::time_point Triggerbot::s_lastClick = std::chrono::steady_clock::now();

static std::mt19937 s_rng(std::random_device{}());

bool Triggerbot::IsVisible(void* player) {
    if (!player) return false;
    
    float playerPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetPlayerPosition(player, playerPos)) {
        return false;
    }
    
    float camPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetCameraPosition(camPos)) {
        return false;
    }
    
    float direction[3] = {
        playerPos[0] - camPos[0],
        playerPos[1] - camPos[1],
        playerPos[2] - camPos[2]
    };
    
    float distance = sqrtf(direction[0]*direction[0] + 
                          direction[1]*direction[1] + 
                          direction[2]*direction[2]);
    
    if (distance < 0.01f) return false;
    
    direction[0] /= distance;
    direction[1] /= distance;
    direction[2] /= distance;
    
    float hitPoint[3] = {0, 0, 0};
    bool hit = Il2CppBridge::Raycast(camPos, direction, distance, hitPoint);
    
    if (!hit) return true;
    
    float hitDistance = sqrtf((hitPoint[0]-camPos[0])*(hitPoint[0]-camPos[0]) +
                             (hitPoint[1]-camPos[1])*(hitPoint[1]-camPos[1]) +
                             (hitPoint[2]-camPos[2])*(hitPoint[2]-camPos[2]));
    
    return hitDistance >= distance - 0.5f;
}

bool Triggerbot::IsCrosshairOnEnemy() {
    // Получаем направление камеры
    float camPos[3] = {0, 0, 0};
    float camForward[3] = {0, 0, 1};
    
    if (!Il2CppBridge::GetCameraPosition(camPos)) return false;
    if (!Il2CppBridge::GetCameraForward(camForward)) return false;
    
    // Raycast вперёд на 1000 единиц
    float hitPoint[3] = {0, 0, 0};
    bool hit = Il2CppBridge::Raycast(camPos, camForward, 1000.0f, hitPoint);
    
    if (!hit) return false;
    
    // Проверяем что попали в игрока
    void* hitPlayer = Il2CppBridge::GetHitPlayer(hitPoint);
    if (!hitPlayer) return false;
    
    // Проверяем что это враг
    if (!Il2CppBridge::IsEnemy(hitPlayer)) return false;
    
    // Проверяем что жив
    if (!Il2CppBridge::IsAlive(hitPlayer)) return false;
    
    // Если включен headOnly - проверяем что попали в голову
    if (s_headOnly) {
        float headPos[3] = {0, 0, 0};
        if (!Il2CppBridge::GetBonePosition(hitPlayer, 0, headPos)) return false;
        
        float dist = sqrtf((hitPoint[0]-headPos[0])*(hitPoint[0]-headPos[0]) +
                          (hitPoint[1]-headPos[1])*(hitPoint[1]-headPos[1]) +
                          (hitPoint[2]-headPos[2])*(hitPoint[2]-headPos[2]));
        
        if (dist > 0.3f) return false; // Не попали в голову
    }
    
    // Проверяем видимость если включено
    if (s_visibilityCheck && !IsVisible(hitPlayer)) {
        return false;
    }
    
    return true;
}

void Triggerbot::PerformClick() {
    // Рандомная задержка
    std::uniform_int_distribution<int> delayDist(s_delayMin, s_delayMax);
    int delay = delayDist(s_rng);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    
    // Шанс промаха
    std::uniform_real_distribution<float> missDist(0.0f, 100.0f);
    if (missDist(s_rng) < s_missChance) {
        return; // Промах
    }
    
    // Симулируем клик мыши
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
    
    s_lastClick = std::chrono::steady_clock::now();
}

void Triggerbot::OnUpdate() {
    if (!s_enabled) return;
    
    // Проверяем что прошло достаточно времени с последнего клика
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastClick);
    if (elapsed.count() < s_delayMax) return;
    
    // Проверяем что прицел на враге
    if (IsCrosshairOnEnemy()) {
        PerformClick();
    }
}

void Triggerbot::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("Delay Min (ms)", &s_delayMin, 10, 500);
        Widgets::Slider("Delay Max (ms)", &s_delayMax, 10, 500);
        Widgets::Slider("Miss Chance (%)", &s_missChance, 0.0f, 50.0f);
        Widgets::Toggle("Head Only", &s_headOnly);
        Widgets::Toggle("Visibility Check", &s_visibilityCheck);
    }
}

void Triggerbot::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Можно рисовать индикатор когда прицел на враге
    if (IsCrosshairOnEnemy()) {
        // Рисуем красный круг в центре экрана
        // Render::DrawCircle(screenWidth/2, screenHeight/2, 10, Color::Red);
    }
}

} // namespace Combat
} // namespace VV
