#pragma once
#include "module.h"
#include <vector>
namespace Features {
    class Registry {
    public:
        static std::vector<Module*> combat;
        static std::vector<Module*> visual;
        static std::vector<Module*> players;
        static std::vector<Module*> auction;
        static std::vector<Module*> world;
        static std::vector<Module*> misc;
        static void Init();
        static void RenderTab(int id);
        static void UpdateAll();
        static void RenderAllOverlays();
    };
}
