// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "silent_aim.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../math/math_utils.h"
#include "../../gui/widgets.h"
#include <windows.h>
#include <algorithm>

namespace VV {
namespace Combat {

bool SilentAim::s_enabled = false;
float SilentAim::s_fov = 5.0f;
bool SilentAim::s_visibilityCheck = true;
bool SilentAim::s_prediction = true;
float SilentAim::s_smoothness = 1.0f;
bool SilentAim::s_jitter = true;
float SilentAim::s_jitterAmount = 0.5f;
SilentAim::Target SilentAim::s_currentTarget = {nullptr, 0, 0.0f, false};

static std::mt19937 s_rng(std::random_device{}());

bool SilentAim::IsVisible(void* player, uint64_t boneIndex) {
    if (!player) return false;
    
    // Получаем позицию кости
    float bonePos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetBonePosition(player, boneIndex, bonePos)) {
        return false;
    }
    
    // Получаем позицию камеры
    float camPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetCameraPosition(camPos)) {
        return false;
    }
    
    // Physics.Raycast для проверки видимости
    float direction[3] = {
        bonePos[0] - camPos[0],
        bonePos[1] - camPos[1],
        bonePos[2] - camPos[2]
    };
    
    float distance = sqrtf(direction[0]*direction[0] + 
                          direction[1]*direction[1] + 
                          direction[2]*direction[2]);
    
    if (distance < 0.01f) return false;
    
    // Нормализуем направление
    direction[0] /= distance;
    direction[1] /= distance;
    direction[2] /= distance;
    
    // Raycast
    float hitPoint[3] = {0, 0, 0};
    bool hit = Il2CppBridge::Raycast(camPos, direction, distance, hitPoint);
    
    // Если ничего не попали или попали в самого игрока - видимо
    if (!hit) return true;
    
    float hitDistance = sqrtf((hitPoint[0]-camPos[0])*(hitPoint[0]-camPos[0]) +
                             (hitPoint[1]-camPos[1])*(hitPoint[1]-camPos[1]) +
                             (hitPoint[2]-camPos[2])*(hitPoint[2]-camPos[2]));
    
    return hitDistance >= distance - 0.5f;
}

void SilentAim::PredictPosition(void* player, float* outPos) {
    if (!player || !outPos) return;
    
    // Получаем текущую позицию
    float currentPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetPlayerPosition(player, currentPos)) {
        outPos[0] = outPos[1] = outPos[2] = 0;
        return;
    }
    
    // Получаем скорость игрока
    float velocity[3] = {0, 0, 0};
    if (!Il2CppBridge::GetPlayerVelocity(player, velocity)) {
        outPos[0] = currentPos[0];
        outPos[1] = currentPos[1];
        outPos[2] = currentPos[2];
        return;
    }
    
    // Получаем время полёта пули (примерное)
    float bulletTime = 0.1f; // 100ms для среднего расстояния
    
    // Предсказываем позицию
    outPos[0] = currentPos[0] + velocity[0] * bulletTime;
    outPos[1] = currentPos[1] + velocity[1] * bulletTime;
    outPos[2] = currentPos[2] + velocity[2] * bulletTime;
}

void SilentAim::ApplyJitter(float& x, float& y, float& z) {
    if (!s_jitter) return;
    
    std::uniform_real_distribution<float> dist(-s_jitterAmount, s_jitterAmount);
    x += dist(s_rng);
    y += dist(s_rng);
    z += dist(s_rng);
}

SilentAim::Target SilentAim::FindBestTarget() {
    Target best = {nullptr, 0, 999999.0f, false};
    
    // Получаем список всех игроков
    auto players = Il2CppBridge::GetAllPlayers();
    
    // Получаем позицию камеры
    float camPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetCameraPosition(camPos)) {
        return best;
    }
    
    // Получаем направление камеры
    float camForward[3] = {0, 0, 1};
    Il2CppBridge::GetCameraForward(camForward);
    
    for (void* player : players) {
        if (!player) continue;
        
        // Проверяем что это враг
        if (!Il2CppBridge::IsEnemy(player)) continue;
        
        // Проверяем что игрок жив
        if (!Il2CppBridge::IsAlive(player)) continue;
        
        // Проверяем кости (голова, шея, грудь)
        uint64_t bones[] = {0, 1, 2}; // Head, Neck, Chest
        for (uint64_t bone : bones) {
            float bonePos[3] = {0, 0, 0};
            if (!Il2CppBridge::GetBonePosition(player, bone, bonePos)) continue;
            
            // Вычисляем расстояние
            float dx = bonePos[0] - camPos[0];
            float dy = bonePos[1] - camPos[1];
            float dz = bonePos[2] - camPos[2];
            float distance = sqrtf(dx*dx + dy*dy + dz*dz);
            
            if (distance < 0.1f) continue;
            
            // Вычисляем угол относительно направления камеры
            float dot = (dx*camForward[0] + dy*camForward[1] + dz*camForward[2]) / distance;
            float angle = acosf(dot) * 180.0f / 3.14159f;
            
            // Проверяем FOV
            if (angle > s_fov) continue;
            
            // Проверяем видимость если включено
            bool visible = true;
            if (s_visibilityCheck) {
                visible = IsVisible(player, bone);
            }
            
            if (!visible) continue;
            
            // Выбираем ближайшую цель
            if (distance < best.distance) {
                best.player = player;
                best.boneIndex = bone;
                best.distance = distance;
                best.visible = visible;
            }
        }
    }
    
    return best;
}

void SilentAim::OverrideShootAngle(float* angle, float targetX, float targetY, float targetZ) {
    if (!angle) return;
    
    // Получаем позицию камеры
    float camPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetCameraPosition(camPos)) return;
    
    // Вычисляем направление к цели
    float dx = targetX - camPos[0];
    float dy = targetY - camPos[1];
    float dz = targetZ - camPos[2];
    
    float distance = sqrtf(dx*dx + dy*dy + dz*dz);
    if (distance < 0.01f) return;
    
    // Вычисляем углы
    float yaw = atan2f(dz, dx) * 180.0f / 3.14159f;
    float pitch = asinf(dy / distance) * 180.0f / 3.14159f;
    
    // Применяем джиттер
    ApplyJitter(yaw, pitch, distance);
    
    // Устанавливаем углы (это подменяет реальные углы выстрела)
    angle[0] = pitch;
    angle[1] = yaw;
}

void SilentAim::OnUpdate() {
    if (!s_enabled) {
        s_currentTarget = {nullptr, 0, 0.0f, false};
        return;
    }
    
    // Ищем лучшую цель
    s_currentTarget = FindBestTarget();
    
    if (!s_currentTarget.player) return;
    
    // Получаем позицию цели
    float targetPos[3] = {0, 0, 0};
    if (s_prediction) {
        PredictPosition(s_currentTarget.player, targetPos);
    } else {
        Il2CppBridge::GetBonePosition(s_currentTarget.player, s_currentTarget.boneIndex, targetPos);
    }
    
    // Хукаем функцию выстрела и подменяем угол
    // Это делается через хук на Weapon::Shoot или подобное
    // Реализация зависит от конкретной игры
}

void SilentAim::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("FOV", &s_fov, 1.0f, 30.0f);
        Widgets::Toggle("Visibility Check", &s_visibilityCheck);
        Widgets::Toggle("Prediction", &s_prediction);
        Widgets::Slider("Smoothness", &s_smoothness, 0.1f, 5.0f);
        Widgets::Toggle("Jitter", &s_jitter);
        if (s_jitter) {
            Widgets::Slider("Jitter Amount", &s_jitterAmount, 0.1f, 2.0f);
        }
    }
}

void SilentAim::OnRenderOverlay() {
    if (!s_enabled || !s_currentTarget.player) return;
    
    // Рисуем линию от центра экрана к цели
    float targetPos[3] = {0, 0, 0};
    Il2CppBridge::GetBonePosition(s_currentTarget.player, s_currentTarget.boneIndex, targetPos);
    
    float screenX, screenY;
    if (Il2CppBridge::WorldToScreen(targetPos, screenX, screenY)) {
        // Рисуем линию (реализация зависит от рендера)
        // Render::DrawLine(screenWidth/2, screenHeight/2, screenX, screenY, Color::Red);
    }
}

} // namespace Combat
} // namespace VV
