// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "anomaly_esp.h"
#include "../../core/offsets.h"
#include "../../render/opengl_hook.h"
#include "../../math/math_utils.h"
#include "../../gui/widgets.h"
#include <cmath>

namespace VV {
namespace Visuals {

bool AnomalyESP::s_enabled = false;
float AnomalyESP::s_thickness = 2.0f;
bool AnomalyESP::s_showName = true;
bool AnomalyESP::s_showRadius = true;
float AnomalyESP::s_color[4] = {1.0f, 0.0f, 0.0f, 1.0f}; // Красный

void AnomalyESP::Render() {
    if (!s_enabled) return;

    // Получаем все аномалии
    auto& anomalies = Core::IL2CPP::GetObjectsByClass("AnomalyTrigger");
    
    for (auto& anom : anomalies) {
        if (!anom.IsValid()) continue;

        // Получаем центр и радиус аномалии
        Vector3 center = anom.GetPosition();
        float radius = anom.GetProperty<float>("DamageRadius");
        
        // Получаем название аномалии
        std::string name = anom.GetProperty<std::string>("AnomalyName");
        
        // Генерируем 8 углов 3D бокса
        Vector3 corners[8];
        Math::GenerateBoxCorners(center, radius, corners);
        
        // Проецируем все углы на экран
        Vector2 screenCorners[8];
        bool allVisible = true;
        
        for(int i = 0; i < 8; ++i) {
            Vector3 sc;
            if(!Render::WorldToScreen(corners[i], sc)) {
                allVisible = false;
                break;
            }
            screenCorners[i] = {sc.x, sc.y};
        }

        // Если не все углы видны - пропускаем
        if (!allVisible) continue;

        // Рисуем красный 3D бокс
        Render::Draw3DBox(screenCorners, s_color, s_thickness);
        
        // Рисуем название если включено
        if (s_showName && !name.empty()) {
            // Центрируем текст над боксом
            Vector2 topCenter = {
                (screenCorners[0].x + screenCorners[1].x) / 2.0f,
                std::min(screenCorners[0].y, screenCorners[1].y) - 10.0f
            };
            Render::DrawText(topCenter, name.c_str(), {1.0f, 1.0f, 1.0f, 1.0f});
        }
        
        // Рисуем радиус поражения если включено
        if (s_showRadius) {
            // Рисуем круг на земле вокруг аномалии
            Vector3 groundCenter = center;
            groundCenter.y = center.y - 0.5f; // Немного ниже центра
            
            std::vector<Vector2> circlePoints;
            for (int i = 0; i < 36; i++) {
                float angle = i * 10.0f * 3.14159f / 180.0f;
                Vector3 point = {
                    groundCenter.x + radius * cosf(angle),
                    groundCenter.y,
                    groundCenter.z + radius * sinf(angle)
                };
                
                Vector3 screenPoint;
                if (Render::WorldToScreen(point, screenPoint)) {
                    circlePoints.push_back({screenPoint.x, screenPoint.y});
                }
            }
            
            if (circlePoints.size() > 2) {
                float radiusColor[4] = {1.0f, 0.5f, 0.0f, 0.5f}; // Оранжевый полупрозрачный
                Render::DrawPolygon(circlePoints, radiusColor, 1.0f);
            }
        }
        
        // Пульсация цвета для привлечения внимания
        float pulse = 0.5f + 0.5f * sinf(Core::Time::GetTime() * 2.0f);
        float pulseColor[4] = {
            s_color[0] * pulse,
            s_color[1] * pulse,
            s_color[2] * pulse,
            s_color[3]
        };
        
        // Рисуем дополнительный пульсирующий контур
        Render::Draw3DBox(screenCorners, pulseColor, s_thickness * 1.5f);
    }
}

void AnomalyESP::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("Thickness", &s_thickness, 1.0f, 5.0f);
        Widgets::Toggle("Show Name", &s_showName);
        Widgets::Toggle("Show Radius", &s_showRadius);
        Widgets::ColorEdit("Color", s_color);
    }
}

} // namespace Visuals
} // namespace VV
