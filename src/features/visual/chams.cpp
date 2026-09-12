// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "chams.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include "../../render/opengl_hook.h"
#include <windows.h>

namespace VV {
namespace Visual {

bool Chams::s_enabled = false;
bool Chams::s_visibleOnly = false;
float Chams::s_colorEnemy[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float Chams::s_colorAlly[4] = {0.0f, 1.0f, 0.0f, 1.0f};
float Chams::s_colorVisible[4] = {1.0f, 1.0f, 0.0f, 1.0f};

void Chams::OnUpdate() {
    if (!s_enabled) return;
    
    // Хукаем функцию рендера игроков
    // Это делается через патч или хук на функцию которая рендерит модель игрока
    // Например, hook на Player::OnRenderObject() или подобное
    
    // Отключаем depth test для врагов чтобы они были видны сквозь стены
    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_DEPTH_TEST);
    
    // Изменяем материал игрока
    // Устанавливаем цвет материала
    // material.SetColor("_Color", color);
}

void Chams::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Toggle("Visible Only", &s_visibleOnly);
        Widgets::ColorEdit("Enemy Color", s_colorEnemy);
        Widgets::ColorEdit("Ally Color", s_colorAlly);
        Widgets::ColorEdit("Visible Color", s_colorVisible);
    }
}

void Chams::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Chams применяются через хуки рендера, а не через overlay
    // Но можно рисовать индикатор что chams активны
}

// Функция которая вызывается из хука рендера игрока
void Chams::ApplyChams(void* player, void* material) {
    if (!s_enabled || !player || !material) return;
    
    // Проверяем что это враг
    bool isEnemy = Il2CppBridge::IsEnemy(player);
    
    // Получаем цвет
    float* color = isEnemy ? s_colorEnemy : s_colorAlly;
    
    // Проверяем видимость если включено
    if (s_visibleOnly) {
        bool visible = Il2CppBridge::IsPlayerVisible(player);
        if (!visible) return;
        color = s_colorVisible;
    }
    
    // Применяем цвет к материалу
    // Il2CppBridge::SetMaterialColor(material, "_Color", color);
    // Il2CppBridge::SetMaterialColor(material, "_EmissionColor", color);
}

} // namespace Visual
} // namespace VV
