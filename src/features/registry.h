#pragma once
#include "module.h"
#include <vector>
namespace Features {
    class Registry {
    public:
        static std::vector<Module*> combat, visual, players, auction, world, misc;
        static void Init();
        static void RenderTab(int id);
        static void UpdateAll();
        static void RenderAllOverlays();
    };
}
