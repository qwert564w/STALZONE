#pragma once
#include "imgui.h"
namespace Widgets {
    bool Toggle(const char* label, bool* v);
    bool Slider(const char* label, float* v, float min, float max);
    bool Keybind(const char* label, int* key);
    void FeatureCard(const char* name, bool enabled);
}
