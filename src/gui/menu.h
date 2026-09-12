// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <string>

namespace VV {
    namespace GUI {
        class Menu {
        public:
            static void Render();
            static void Init();
            static bool s_isOpen;
        private:
            static void RenderVisualTab();
            static void RenderMiscTab();
            static void RenderBypassTab();
        };
    }
}
