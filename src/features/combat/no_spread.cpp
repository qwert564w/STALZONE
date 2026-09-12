// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "no_spread.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include <windows.h>

namespace VV {
namespace Combat {

bool NoSpread::s_enabled = false;
float NoSpread::s_reductionPercent = 100.0f;

void NoSpread::ReduceSpread() {
    // Получаем текущее оружие
    void* weapon = Il2CppBridge::GetCurrentWeapon();
    if (!weapon) return;
    
    // Получаем текущий spread angle
    float currentSpread = Il2CppBridge::GetWeaponSpread(weapon);
    
    // Вычисляем новый spread
    float reduction = currentSpread * (s_reductionPercent / 100.0f);
    float newSpread = currentSpread - reduction;
    
    // Устанавливаем новый spread
    Il2CppBridge::SetWeaponSpread(weapon, newSpread);
}

void NoSpread::OnUpdate() {
    if (!s_enabled) return;
    
    // Проверяем что игрок стреляет
    bool isShooting = Il2CppBridge::IsShooting();
    
    if (isShooting) {
        ReduceSpread();
    }
}

void NoSpread::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("Reduction (%)", &s_reductionPercent, 0.0f, 100.0f);
    }
}

void NoSpread::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Можно рисовать информацию о spread
    void* weapon = Il2CppBridge::GetCurrentWeapon();
    if (weapon) {
        float spread = Il2CppBridge::GetWeaponSpread(weapon);
        // Render::DrawText("Spread: " + std::to_string(spread), 10, 30);
    }
}

} // namespace Combat
} // namespace VV
