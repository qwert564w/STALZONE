// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "loot_esp.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include "../../render/opengl_hook.h"
#include <cmath>
#include <string>
#include <vector>

namespace VV {
namespace Visual {

bool LootESP::s_enabled = false;
int LootESP::s_minRarity = 1;
float LootESP::s_maxDistance = 150.0f;
bool LootESP::s_showIcons = true;
bool LootESP::s_showNames = true;
float LootESP::s_colorCommon[4] = {0.5f, 0.5f, 0.5f, 1.0f};
float LootESP::s_colorRare[4] = {0.0f, 1.0f, 0.0f, 1.0f};
float LootESP::s_colorEpic[4] = {0.0f, 0.5f, 1.0f, 1.0f};
float LootESP::s_colorLegendary[4] = {1.0f, 0.8f, 0.0f, 1.0f};

void LootESP::OnUpdate() {
    // Ничего не делаем в update
}

void LootESP::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("Min Rarity", &s_minRarity, 1, 5);
        Widgets::Slider("Max Distance", &s_maxDistance, 10.0f, 500.0f);
        Widgets::Toggle("Show Icons", &s_showIcons);
        Widgets::Toggle("Show Names", &s_showNames);
        Widgets::ColorEdit("Common Color", s_colorCommon);
        Widgets::ColorEdit("Rare Color", s_colorRare);
        Widgets::ColorEdit("Epic Color", s_colorEpic);
        Widgets::ColorEdit("Legendary Color", s_colorLegendary);
    }
}

void LootESP::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Получаем список всего лута на карте
    auto lootItems = Il2CppBridge::GetAllLoot();
    
    // Получаем позицию локального игрока
    float localPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetLocalPlayerPosition(localPos)) return;
    
    for (const auto& item : lootItems) {
        // Фильтр по редкости
        if (item.rarity < s_minRarity) continue;
        
        // Получаем позицию лута
        float itemPos[3] = {item.posX, item.posY, item.posZ};
        
        // Вычисляем расстояние
        float distance = sqrtf((itemPos[0]-localPos[0])*(itemPos[0]-localPos[0]) +
                              (itemPos[1]-localPos[1])*(itemPos[1]-localPos[1]) +
                              (itemPos[2]-localPos[2])*(itemPos[2]-localPos[2]));
        
        // Пропускаем слишком далёкий лут
        if (distance > s_maxDistance) continue;
        
        // Проецируем на экран
        float screenPos[2] = {0, 0};
        if (!Il2CppBridge::WorldToScreen(itemPos, screenPos[0], screenPos[1])) continue;
        
        // Выбираем цвет в зависимости от редкости
        float* color = s_colorCommon;
        if (item.rarity == 2) color = s_colorRare;
        if (item.rarity == 3) color = s_colorEpic;
        if (item.rarity >= 4) color = s_colorLegendary;
        
        // Рисуем иконку если включено
        if (s_showIcons) {
            float iconSize = 15.0f;
            
            // Разные иконки для разных типов предметов
            std::string itemType = item.type;
            
            if (itemType == "weapon") {
                // Рисуем иконку оружия (простой прямоугольник)
                Render::DrawRect(screenPos[0] - iconSize/2, screenPos[1] - iconSize, 
                               iconSize, iconSize * 2, color, 2.0f);
            } else if (itemType == "ammo") {
                // Рисуем иконку патронов (маленький прямоугольник)
                Render::DrawRect(screenPos[0] - iconSize/3, screenPos[1] - iconSize/2, 
                               iconSize * 2/3, iconSize, color, 2.0f);
            } else if (itemType == "medical") {
                // Рисуем иконку аптечки (крест)
                float crossSize = iconSize / 3;
                Render::DrawRect(screenPos[0] - crossSize/2, screenPos[1] - iconSize/2, 
                               crossSize, iconSize, color, 2.0f);
                Render::DrawRect(screenPos[0] - iconSize/2, screenPos[1] - crossSize/2, 
                               iconSize, crossSize, color, 2.0f);
            } else if (itemType == "armor") {
                // Рисуем иконку брони (щит)
                Render::DrawCircle(screenPos[0], screenPos[1], iconSize/2, color, 2.0f);
            } else {
                // Обычный предмет (квадрат)
                Render::DrawRect(screenPos[0] - iconSize/2, screenPos[1] - iconSize/2, 
                               iconSize, iconSize, color, 2.0f);
            }
        }
        
        // Рисуем название если включено
        if (s_showNames) {
            std::string displayName = item.name;
            
            // Добавляем количество если больше 1
            if (item.count > 1) {
                displayName += " x" + std::to_string(item.count);
            }
            
            // Рисуем текст над иконкой
            float textY = screenPos[1] - 25.0f;
            Render::DrawText(screenPos[0], textY, displayName.c_str(), color);
        }
        
        // Рисуем расстояние
        char distText[32];
        snprintf(distText, sizeof(distText), "%.0fm", distance);
        Render::DrawText(screenPos[0], screenPos[1] + 20.0f, distText, color);
        
        // Для очень редких предметов рисуем пульсирующий эффект
        if (item.rarity >= 4) {
            float pulse = 0.5f + 0.5f * sinf(Core::Time::GetTime() * 3.0f);
            float pulseColor[4] = {color[0] * pulse, color[1] * pulse, color[2] * pulse, color[3]};
            
            Render::DrawCircle(screenPos[0], screenPos[1], 20.0f, pulseColor, 3.0f);
        }
    }
}

} // namespace Visual
} // namespace VV
