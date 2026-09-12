// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "corpse_esp.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include "../../render/opengl_hook.h"
#include <cmath>
#include <string>

namespace VV {
namespace Visual {

bool CorpseESP::s_enabled = false;
bool CorpseESP::s_showLoot = true;
bool CorpseESP::s_showSkull = true;
float CorpseESP::s_color[4] = {0.5f, 0.5f, 0.5f, 1.0f};
float CorpseESP::s_maxDistance = 100.0f;

void CorpseESP::OnUpdate() {
    // Ничего не делаем в update
}

void CorpseESP::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Toggle("Show Loot", &s_showLoot);
        Widgets::Toggle("Show Skull", &s_showSkull);
        Widgets::Slider("Max Distance", &s_maxDistance, 10.0f, 300.0f);
        Widgets::ColorEdit("Color", s_color);
    }
}

void CorpseESP::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Получаем список всех трупов
    auto corpses = Il2CppBridge::GetAllCorpses();
    
    // Получаем позицию локального игрока
    float localPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetLocalPlayerPosition(localPos)) return;
    
    for (void* corpse : corpses) {
        if (!corpse) continue;
        
        // Получаем позицию трупа
        float corpsePos[3] = {0, 0, 0};
        if (!Il2CppBridge::GetCorpsePosition(corpse, corpsePos)) continue;
        
        // Вычисляем расстояние
        float distance = sqrtf((corpsePos[0]-localPos[0])*(corpsePos[0]-localPos[0]) +
                              (corpsePos[1]-localPos[1])*(corpsePos[1]-localPos[1]) +
                              (corpsePos[2]-localPos[2])*(corpsePos[2]-localPos[2]));
        
        // Пропускаем слишком далёкие трупы
        if (distance > s_maxDistance) continue;
        
        // Проецируем на экран
        float screenPos[2] = {0, 0};
        if (!Il2CppBridge::WorldToScreen(corpsePos, screenPos[0], screenPos[1])) continue;
        
        // Рисуем иконку черепа если включено
        if (s_showSkull) {
            float skullSize = 20.0f;
            // Рисуем простой череп (можно заменить на текстуру)
            float skullX = screenPos[0] - skullSize / 2.0f;
            float skullY = screenPos[1] - skullSize - 5.0f;
            
            // Круг для головы
            Render::DrawCircle(skullX + skullSize / 2.0f, skullY + skullSize / 2.0f, 
                             skullSize / 2.0f, s_color, 2.0f);
            
            // Глаза
            float eyeSize = skullSize * 0.15f;
            float eyeColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
            Render::DrawCircle(skullX + skullSize * 0.35f, skullY + skullSize * 0.4f, 
                             eyeSize, eyeColor, 1.0f);
            Render::DrawCircle(skullX + skullSize * 0.65f, skullY + skullSize * 0.4f, 
                             eyeSize, eyeColor, 1.0f);
            
            // Нос
            float noseX = skullX + skullSize * 0.5f;
            float noseY = skullY + skullSize * 0.6f;
            Render::DrawLine(noseX - 2.0f, noseY, noseX + 2.0f, noseY + 3.0f, s_color, 1.0f);
            Render::DrawLine(noseX + 2.0f, noseY, noseX + 2.0f, noseY + 3.0f, s_color, 1.0f);
            
            // Зубы
            for (int i = 0; i < 4; i++) {
                float toothX = skullX + skullSize * (0.3f + i * 0.1f);
                float toothY = skullY + skullSize * 0.75f;
                Render::DrawLine(toothX, toothY, toothX, toothY + 3.0f, s_color, 1.0f);
            }
        }
        
        // Рисуем лут если включено
        if (s_showLoot) {
            // Получаем список лута в трупе
            auto lootItems = Il2CppBridge::GetCorpseLoot(corpse);
            
            float textY = screenPos[1] + 10.0f;
            float textX = screenPos[0] + 15.0f;
            
            for (const auto& item : lootItems) {
                std::string itemName = item.name;
                int rarity = item.rarity;
                
                // Цвет зависит от редкости
                float itemColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
                if (rarity == 1) itemColor[0] = 0.5f; itemColor[1] = 0.5f; itemColor[2] = 0.5f; // Серый
                if (rarity == 2) itemColor[0] = 0.0f; itemColor[1] = 1.0f; itemColor[2] = 0.0f; // Зелёный
                if (rarity == 3) itemColor[0] = 0.0f; itemColor[1] = 0.5f; itemColor[2] = 1.0f; // Синий
                if (rarity == 4) itemColor[0] = 0.8f; itemColor[1] = 0.2f; itemColor[2] = 1.0f; // Фиолетовый
                if (rarity == 5) itemColor[0] = 1.0f; itemColor[1] = 0.8f; itemColor[2] = 0.0f; // Золотой
                
                // Рисуем название предмета
                Render::DrawText(textX, textY, itemName.c_str(), itemColor);
                textY += 15.0f;
                
                // Показываем максимум 5 предметов
                if (textY > screenPos[1] + 85.0f) break;
            }
        }
        
        // Рисуем расстояние
        char distText[32];
        snprintf(distText, sizeof(distText), "%.0fm", distance);
        Render::DrawText(screenPos[0], screenPos[1] + 5.0f, distText, s_color);
    }
}

} // namespace Visual
} // namespace VV
