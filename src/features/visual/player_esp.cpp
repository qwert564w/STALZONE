// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "player_esp.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../math/math_utils.h"
#include "../../gui/widgets.h"
#include "../../render/opengl_hook.h"
#include <cmath>
#include <string>

namespace VV {
namespace Visual {

bool PlayerESP::s_enabled = false;
bool PlayerESP::s_showBoxes = true;
bool PlayerESP::s_showNames = true;
bool PlayerESP::s_showHealth = true;
bool PlayerESP::s_showDistance = true;
bool PlayerESP::s_showBones = false;
float PlayerESP::s_colorEnemy[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float PlayerESP::s_colorAlly[4] = {0.0f, 1.0f, 0.0f, 1.0f};

void PlayerESP::OnUpdate() {
    // Ничего не делаем в update, всё в render
}

void PlayerESP::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Toggle("Show Boxes", &s_showBoxes);
        Widgets::Toggle("Show Names", &s_showNames);
        Widgets::Toggle("Show Health", &s_showHealth);
        Widgets::Toggle("Show Distance", &s_showDistance);
        Widgets::Toggle("Show Bones", &s_showBones);
        Widgets::ColorEdit("Enemy Color", s_colorEnemy);
        Widgets::ColorEdit("Ally Color", s_colorAlly);
    }
}

void PlayerESP::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Получаем список всех игроков
    auto players = Il2CppBridge::GetAllPlayers();
    
    // Получаем позицию локального игрока
    float localPos[3] = {0, 0, 0};
    if (!Il2CppBridge::GetLocalPlayerPosition(localPos)) return;
    
    for (void* player : players) {
        if (!player) continue;
        
        // Пропускаем локального игрока
        if (Il2CppBridge::IsLocalPlayer(player)) continue;
        
        // Проверяем что игрок жив
        if (!Il2CppBridge::IsAlive(player)) continue;
        
        // Получаем позицию игрока
        float playerPos[3] = {0, 0, 0};
        if (!Il2CppBridge::GetPlayerPosition(player, playerPos)) continue;
        
        // Вычисляем расстояние
        float distance = sqrtf((playerPos[0]-localPos[0])*(playerPos[0]-localPos[0]) +
                              (playerPos[1]-localPos[1])*(playerPos[1]-localPos[1]) +
                              (playerPos[2]-localPos[2])*(playerPos[2]-localPos[2]));
        
        // Пропускаем слишком далёких игроков
        if (distance > 500.0f) continue;
        
        // Получаем цвет в зависимости от команды
        bool isEnemy = Il2CppBridge::IsEnemy(player);
        float* color = isEnemy ? s_colorEnemy : s_colorAlly;
        
        // Получаем bounding box игрока
        float headPos[3] = {0, 0, 0};
        float feetPos[3] = {0, 0, 0};
        
        if (!Il2CppBridge::GetBonePosition(player, 0, headPos)) continue; // Голова
        if (!Il2CppBridge::GetBonePosition(player, 10, feetPos)) continue; // Ноги
        
        // Проецируем на экран
        float screenHead[2] = {0, 0};
        float screenFeet[2] = {0, 0};
        
        if (!Il2CppBridge::WorldToScreen(headPos, screenHead[0], screenHead[1])) continue;
        if (!Il2CppBridge::WorldToScreen(feetPos, screenFeet[0], screenFeet[1])) continue;
        
        // Вычисляем размеры бокса
        float height = screenFeet[1] - screenHead[1];
        float width = height * 0.4f; // Соотношение сторон
        
        float boxX = screenHead[0] - width / 2.0f;
        float boxY = screenHead[1];
        
        // Рисуем бокс если включено
        if (s_showBoxes) {
            Render::DrawRect(boxX, boxY, width, height, color, 2.0f);
            
            // Рисуем угловые линии для стиля
            float cornerLength = width * 0.2f;
            Render::DrawLine(boxX, boxY, boxX + cornerLength, boxY, color, 3.0f);
            Render::DrawLine(boxX, boxY, boxX, boxY + cornerLength, color, 3.0f);
            
            Render::DrawLine(boxX + width, boxY, boxX + width - cornerLength, boxY, color, 3.0f);
            Render::DrawLine(boxX + width, boxY, boxX + width, boxY + cornerLength, color, 3.0f);
            
            Render::DrawLine(boxX, boxY + height, boxX + cornerLength, boxY + height, color, 3.0f);
            Render::DrawLine(boxX, boxY + height, boxX, boxY + height - cornerLength, color, 3.0f);
            
            Render::DrawLine(boxX + width, boxY + height, boxX + width - cornerLength, boxY + height, color, 3.0f);
            Render::DrawLine(boxX + width, boxY + height, boxX + width, boxY + height - cornerLength, color, 3.0f);
        }
        
        // Рисуем имя если включено
        if (s_showNames) {
            std::string name = Il2CppBridge::GetPlayerName(player);
            float textY = boxY - 15.0f;
            Render::DrawText(boxX + width / 2.0f, textY, name.c_str(), color);
        }
        
        // Рисуем здоровье если включено
        if (s_showHealth) {
            int health = Il2CppBridge::GetPlayerHealth(player);
            float healthPercent = health / 100.0f;
            
            // Рисуем health bar слева от бокса
            float barX = boxX - 5.0f;
            float barY = boxY;
            float barWidth = 3.0f;
            float barHeight = height;
            
            // Фон
            float bgColor[4] = {0.2f, 0.2f, 0.2f, 0.8f};
            Render::DrawRect(barX, barY, barWidth, barHeight, bgColor, 1.0f);
            
            // Заполнение
            float fillColor[4] = {0.0f, 1.0f, 0.0f, 1.0f};
            if (healthPercent < 0.5f) fillColor[1] = healthPercent * 2.0f; // Жёлтый->Красный
            if (healthPercent < 0.25f) fillColor[0] = 1.0f;
            
            float fillHeight = barHeight * healthPercent;
            Render::DrawRect(barX, barY + barHeight - fillHeight, barWidth, fillHeight, fillColor, 1.0f);
        }
        
        // Рисуем расстояние если включено
        if (s_showDistance) {
            char distText[32];
            snprintf(distText, sizeof(distText), "%.0fm", distance);
            float textY = boxY + height + 5.0f;
            Render::DrawText(boxX + width / 2.0f, textY, distText, color);
        }
        
        // Рисуем кости если включено
        if (s_showBones) {
            // Рисуем линии между основными костями
            uint64_t bonePairs[][2] = {
                {0, 1},   // Голова -> Шея
                {1, 2},   // Шея -> Грудь
                {2, 3},   // Грудь -> Позвоночник
                {3, 4},   // Позвоночник -> Таз
                {4, 5},   // Таз -> Левое бедро
                {4, 8},   // Таз -> Правое бедро
                {1, 11},  // Шея -> Левое плечо
                {1, 15},  // Шея -> Правое плечо
                {11, 12}, // Левое плечо -> Локоть
                {15, 16}, // Правое плечо -> Локоть
                {12, 13}, // Локоть -> Кисть
                {16, 17}, // Локоть -> Кисть
                {5, 6},   // Левое бедро -> Колено
                {8, 9},   // Правое бедро -> Колено
                {6, 7},   // Колено -> Стопа
                {9, 10}   // Колено -> Стопа
            };
            
            for (auto& pair : bonePairs) {
                float pos1[3] = {0, 0, 0};
                float pos2[3] = {0, 0, 0};
                
                if (Il2CppBridge::GetBonePosition(player, pair[0], pos1) &&
                    Il2CppBridge::GetBonePosition(player, pair[1], pos2)) {
                    
                    float screen1[2] = {0, 0};
                    float screen2[2] = {0, 0};
                    
                    if (Il2CppBridge::WorldToScreen(pos1, screen1[0], screen1[1]) &&
                        Il2CppBridge::WorldToScreen(pos2, screen2[0], screen2[1])) {
                        
                        float boneColor[4] = {1.0f, 1.0f, 0.0f, 1.0f}; // Жёлтый
                        Render::DrawLine(screen1[0], screen1[1], screen2[0], screen2[1], boneColor, 2.0f);
                    }
                }
            }
        }
    }
}

} // namespace Visual
} // namespace VV
