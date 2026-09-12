// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "artifact_glow.h"
#include "../../core/offsets.h"
#include "../../render/opengl_hook.h"
#include "../../gui/widgets.h"
#include <cmath>
#include <vector>

namespace VV {
namespace Visuals {

bool ArtifactGlow::s_enabled = false;
float ArtifactGlow::s_intensity = 1.5f;
float ArtifactGlow::s_radius = 50.0f;
bool ArtifactGlow::s_rareOnly = false;
float ArtifactGlow::s_color[4] = {1.0f, 0.8f, 0.2f, 1.0f}; // Золотой по умолчанию
float ArtifactGlow::s_maxDistance = 200.0f;

void ArtifactGlow::Render() {
    if (!s_enabled) return;

    // Получаем все объекты класса ArtifactBase
    auto& objects = Core::IL2CPP::GetObjectsByClass("ArtifactBase");
    
    for (auto& obj : objects) {
        if (!obj.IsValid()) continue;

        // Получаем редкость артефакта
        int rarity = obj.GetProperty<int>("Rarity");
        
        // Фильтр по редкости
        if (s_rareOnly && rarity < 3) continue;

        // Получаем позицию артефакта
        Vector3 pos = obj.GetPosition();
        
        // Вычисляем расстояние от игрока
        float dist = Core::Math::Distance(Core::LocalPlayer::GetPosition(), pos);
        
        // Проверка максимальной дистанции
        if (dist > s_maxDistance) continue;

        // Проецируем на экран
        Vector3 screenPos;
        if (Render::WorldToScreen(pos, screenPos)) {
            // Вычисляем прозрачность в зависимости от расстояния
            float alpha = 1.0f - (dist / s_maxDistance);
            alpha = std::max(0.0f, std::min(1.0f, alpha));
            
            // Цвет зависит от редкости
            float color[4] = {s_color[0], s_color[1], s_color[2], alpha * s_intensity};
            
            // Редкие артефакты светятся ярче
            if (rarity >= 4) {
                color[3] *= 1.5f; // Увеличиваем яркость
            }
            
            // Рисуем светящийся круг
            Render::DrawGlowCircle(screenPos, s_radius, color);
            
            // Дополнительный эффект для очень редких
            if (rarity >= 5) {
                // Рисуем второй круг с пульсацией
                float pulse = 0.5f + 0.5f * sinf(Core::Time::GetTime() * 3.0f);
                float pulseColor[4] = {1.0f, 1.0f, 1.0f, pulse * alpha * s_intensity};
                Render::DrawGlowCircle(screenPos, s_radius * 1.2f, pulseColor);
            }
        }
    }
}

void ArtifactGlow::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("Intensity", &s_intensity, 0.1f, 3.0f);
        Widgets::Slider("Radius", &s_radius, 10.0f, 100.0f);
        Widgets::Slider("Max Distance", &s_maxDistance, 50.0f, 500.0f);
        Widgets::Toggle("Rare Only", &s_rareOnly);
        Widgets::ColorEdit("Color", s_color);
    }
}

} // namespace Visuals
} // namespace VV
