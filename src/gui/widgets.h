#pragma once
#include <imgui.h>
#include <string>
namespace Widgets {
    bool Toggle(const char* id, bool* value);
    bool Slider(const char* id, const char* label, float* value, float min, float max, const char* fmt = "%.1f");
    bool SliderInt(const char* id, const char* label, int* value, int min, int max, const char* fmt = "%d");
    bool FeatureCardStart(const char* id, const char* name, bool* enabled, bool* expanded);
    void FeatureCardEnd();
    bool SettingRow(const char* label);
    void SectionGroup(const char* label);
}