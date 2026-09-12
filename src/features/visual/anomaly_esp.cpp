// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "anomaly_esp.h"
#include "../../core/offsets.h"
#include "../../render/opengl_hook.h"
#include "../../math/math_utils.h"

namespace VV {
    namespace Visuals {
        bool AnomalyESP::s_enabled = false;
        float AnomalyESP::s_thickness = 2.0f;
        bool AnomalyESP::s_showName = true;
        bool AnomalyESP::s_showRadius = true;

        void AnomalyESP::Render() {
            if (!s_enabled) return;

            auto& anomalies = Core::IL2CPP::GetObjectsByClass("AnomalyTrigger");
            for (auto& anom : anomalies) {
                if (!anom.IsValid()) continue;

                Vector3 center = anom.GetPosition();
                float radius = anom.GetProperty<float>("DamageRadius");
                
                // 3D Bounding Box projection
                Vector3 corners[8];
                Math::GenerateBoxCorners(center, radius, corners);
                
                Vector2 screenCorners[8];
                bool allVisible = true;
                for(int i=0; i<8; ++i) {
                    Vector3 sc;
                    if(!Render::WorldToScreen(corners[i], sc)) {
                        allVisible = false;
                        break;
                    }
                    screenCorners[i] = {sc.x, sc.y};
                }

                if (!allVisible) continue;

                // Draw red hitbox
                Render::Draw3DBox(screenCorners, {1.0f, 0.0f, 0.0f, 0.0f;
            }
        }
    }
