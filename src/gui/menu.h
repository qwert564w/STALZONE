#pragma once
#include <imgui.h>
namespace Menu {
    extern int currentTab;
    extern bool visible;
    void Init();
    void Render();
    void Shutdown();
}