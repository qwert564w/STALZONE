// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "no_recoil.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include <windows.h>
#include <random>
#include <cmath>

namespace VV {
namespace Combat {

bool NoRecoil::s_enabled = false;
float NoRecoil::s_strength = 95.0f;
bool NoRecoil::s_verticalOnly = false;
bool NoRecoil::s_horizontalOnly = false;
float NoRecoil::s_lastPunchAngleX = 0.0f;
float NoRecoil::s_lastPunchAngleY = 0.0f;

static std::mt19937 s_rng(std::random_device{}());

void NoRecoil::CompensateRecoil() {
    // Получаем текущий угол камеры
    float currentAngle[2] = {0, 0};
    if (!Il2CppBridge::GetViewAngles(currentAngle)) return;
    
    // Получаем punch angle (отдачу)
    float punchAngle[2] = {0, 0};
    if (!Il2CppBridge::GetPunchAngle(punchAngle)) return;
    
    // Вычисляем дельту отдачи
    float deltaX = punchAngle[0] - s_lastPunchAngleX;
    float deltaY = punchAngle[1] - s_lastPunchAngleY;
    
    // Рандомизация силы компенсации (95-100%)
    std::uniform_real_distribution<float> strengthDist(0.95f, 1.0f);
    float randomStrength = strengthDist(s_rng) * (s_strength / 100.0f);
    
    // Применяем компенсацию
    float compensationX = 0.0f;
    float compensationY = 0.0f;
    
    if (!s_horizontalOnly) {
        compensationX = deltaX * randomStrength;
    }
    
    if (!s_verticalOnly) {
        compensationY = deltaY * randomStrength;
    }
    
    // Применяем компенсацию к текущему углу
    float newAngle[2] = {
        currentAngle[0] - compensationX,
        currentAngle[1] - compensationY
    };
    
    // Устанавливаем новый угол
    Il2CppBridge::SetViewAngles(newAngle);
    
    // Запоминаем текущий punch angle для следующего кадра
    s_lastPunchAngleX = punchAngle[0];
    s_lastPunchAngleY = punchAngle[1];
}

void NoRecoil::OnUpdate() {
    if (!s_enabled) return;
    
    // Проверяем что игрок стреляет
    bool isShooting = Il2CppBridge::IsShooting();
    
    if (isShooting) {
        CompensateRecoil();
    } else {
        // Сбрасываем punch angle когда не стреляем
        float punchAngle[2] = {0, 0};
        Il2CppBridge::GetPunchAngle(punchAngle);
        s_lastPunchAngleX = punchAngle[0];
        s_lastPunchAngleY = punchAngle[1];
    }
}

void NoRecoil::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("Strength (%)", &s_strength, 0.0f, 100.0f);
        Widgets::Toggle("Vertical Only", &s_verticalOnly);
        Widgets::Toggle("Horizontal Only", &s_horizontalOnly);
    }
}

void NoRecoil::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Можно рисовать информацию о компенсации
    float punchAngle[2] = {0, 0};
    Il2CppBridge::GetPunchAngle(punchAngle);
    
    // Render::DrawText("Recoil: " + std::to_string(punchAngle[0]), 10, 10);
}

} // namespace Combat
} // namespace VV
