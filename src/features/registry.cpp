#include "registry.h"
#include "imgui.h"
#include "../gui/widgets.h"
namespace Features {
    std::vector<Module*> Registry::combat, Registry::visual, Registry::players, Registry::auction, Registry::world, Registry::misc;
    void Registry::Init() { }
    void Registry::RenderTab(int id) {
        std::vector<Module*>* list = nullptr;
        if (id == 0) list = &combat; else if (id == 1) list = &visual; else if (id == 2) list = &players;
        else if (id == 3) list = &auction; else if (id == 4) list = &world; else if (id == 5) list = &misc;
        if (list) { for (auto* m : *list) { Widgets::Toggle(m->name.c_str(), &m->enabled); m->OnRenderMenu(); } }
    }
    void Registry::UpdateAll() { }
    void Registry::RenderAllOverlays() { }
}
