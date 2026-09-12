// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "artifact_glow.h"
#include "../../core/offsets.h"
#include "../../render/opengl_hook.h"
#include <cmath>

namespace VV {
    namespace Visuals {
        bool ArtifactGlow::s_enabled = false;
        float ArtifactGlow::s_intensity = 1.5f;
        float ArtifactGlow::s_radius = 50.0f;
        bool ArtifactGlow::s_rareOnly = false;

        void ArtifactGlow::Render() {
            if (!s_enabled) return;

            auto& objects = Core::IL2CPP::GetObjectsByClass("ArtifactBase");
            for (auto& obj : objects) {
                if (!obj.IsValid()) continue;

                int rarity = obj.GetProperty<int>("Rarity");
                if (s_rareOnly && rarity < 3) continue;

                Vector3 pos = obj.GetPosition();
                Vector3 screenPos;
                if (Render::WorldToScreen(pos, screenPos)) {
                    // Apply glow effect via custom shader or overlay
                    float dist = Core::Math::Distance(Core::LocalPlayer::GetPosition(), pos);
                    if (dist > 200.0f) continue;

                    // Draw glowing circle
                    float alpha = 1.0f - (dist / 200.0f);
                    Render::DrawGlowCircle(screenPos, s_radius, {1.0f, 0.8f, 0.2f, alpha * s_intensity});
                }
            }
        }
    }
}
