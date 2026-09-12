// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "fov_circle.h"
#include "../../core/offsets.h"
#include "../../core/il2cpp_bridge.h"
#include "../../gui/widgets.h"
#include "../../render/opengl_hook.h"
#include <windows.h>
#include <cmath>

namespace VV {
namespace Combat {

bool FovCircle::s_enabled = false;
float FovCircle::s_fov = 5.0f;
float FovCircle::s_color[4] = {1.0f, 0.0f, 0.0f, 1.0f}; // Красный
float FovCircle::s_thickness = 2.0f;

void FovCircle::OnUpdate() {
    // Ничего не делаем в update
}

void FovCircle::OnRenderMenu() {
    Widgets::Toggle("Enabled", &s_enabled);
    if (s_enabled) {
        Widgets::Slider("FOV", &s_fov, 1.0f, 30.0f);
        Widgets::ColorEdit("Color", s_color);
        Widgets::Slider("Thickness", &s_thickness, 1.0f, 5.0f);
    }
}

void FovCircle::OnRenderOverlay() {
    if (!s_enabled) return;
    
    // Получаем размеры экрана
    int screenWidth = 1920; // Нужно получить реальные размеры
    int screenHeight = 1080;
    
    // Вычисляем радиус круга в пикселях
    // FOV в градусах -> радиус в пикселях
    // Примерная формула: radius = (fov / 90.0) * (screenWidth / 2)
    float radius = (s_fov / 90.0f) * (screenWidth / 2.0f);
    
    // Центрируем круг
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    
    // Рисуем круг
    // Render::DrawCircle(centerX, centerY, radius, s_color, s_thickness);
    
    // Временная реализация через OpenGL (если нет Render класса)
    // glBegin(GL_LINE_LOOP);
    // glColor4fv(s_color);
    // for (int i = 0; i < 360; i++) {
    //     float angle = i * 3.14159f / 180.0f;
    //     float x = centerX + radius * cosf(angle);
    //     float y = centerY + radius * sinf(angle);
    //     glVertex2f(x, y);
    // }
    // glEnd();
}

} // namespace Combat
} // namespace VV
