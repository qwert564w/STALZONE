// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <vector>
#include "../../core/il2cpp_bridge.h"

namespace VV {
    namespace Visuals {
        class ArtifactGlow {
        public:
            static void Render();
            static bool s_enabled;
            static float s_intensity;
            static float s_radius;
            static bool s_rareOnly;
        };
    }
}
