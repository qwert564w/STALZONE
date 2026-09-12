// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "menu.h"
#include "widgets.h"
#include "theme.h"
#include "../features/combat/aimbot.h"
#include "../features/combat/silent_aim.h"
#include "../features/combat/triggerbot.h"
#include "../features/combat/no_recoil.h"
#include "../features/combat/no_spread.h"
#include "../features/combat/fov_circle.h"
#include "../features/visual/artifact_glow.h"
#include "../features/visual/anomaly_esp.h"
#include "../features/visual/player_esp.h"
#include "../features/visual/chams.h"
#include "../features/visual/corpse_esp.h"
#include "../features/visual/loot_esp.h"
#include <windows.h>

namespace VV {
namespace GUI {

bool s_menuOpen = false;
int s_currentTab = 0;
bool s_initialized = false;

void ToggleMenu() {
    s_menuOpen = !s_menuOpen;
}

bool IsMenuOpen() {
    return s_menuOpen;
}

void HandleInput() {
    if (GetAsyncKeyState(VK_RSHIFT) & 0x8000) {
        static bool lastState = false;
        if (!lastState) {
            ToggleMenu();
        }
        lastState = true;
    } else {
        if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
            static bool lastState2 = false;
            if (!lastState2) {
                ToggleMenu();
            }
            lastState2 = true;
        } else {
            static bool lastState = false;
            static bool lastState2 = false;
            lastState = false;
            lastState2 = false;
        }
    }
    
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        if (s_menuOpen) {
            s_menuOpen = false;
        }
    }
}

void RenderMenu() {
    HandleInput();
    
    if (!s_menuOpen) return;
    
    float menuX = 100.0f;
    float menuY = 100.0f;
    float menuWidth = 600.0f;
    float menuHeight = 500.0f;
    
    Widgets::WindowStart("VV.EXE Menu [Right Shift]", menuX, menuY, menuWidth, menuHeight);
    
    const char* tabs[] = {"Combat", "Visual", "Misc", "Bypass", "Settings"};
    int numTabs = sizeof(tabs) / sizeof(tabs[0]);
    
    for (int i = 0; i < numTabs; i++) {
        if (Widgets::Tab(tabs[i], i == s_currentTab)) {
            s_currentTab = i;
        }
    }
    
    if (s_currentTab == 0) {
        Widgets::FeatureCardStart("Aimbot");
        Combat::Aimbot::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Silent Aim");
        Combat::SilentAim::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Triggerbot");
        Combat::Triggerbot::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("No Recoil");
        Combat::NoRecoil::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("No Spread");
        Combat::NoSpread::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("FOV Circle");
        Combat::FovCircle::OnRenderMenu();
        Widgets::FeatureCardEnd();
    }
    else if (s_currentTab == 1) {
        Widgets::FeatureCardStart("Player ESP");
        Visual::PlayerESP::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Loot ESP");
        Visual::LootESP::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Corpse ESP");
        Visual::CorpseESP::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Chams");
        Visual::Chams::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Artifact Glow");
        Visual::ArtifactGlow::OnRenderMenu();
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Anomaly ESP");
        Visual::AnomalyESP::OnRenderMenu();
        Widgets::FeatureCardEnd();
    }
    else if (s_currentTab == 2) {
        Widgets::Text("Misc features coming soon...");
    }
    else if (s_currentTab == 3) {
        Widgets::FeatureCardStart("Anti-Cheat Bypass");
        Widgets::Text("Select bypass strategy:");
        static int bypassMode = 0;
        Widgets::RadioButton("Manual Map", &bypassMode, 0);
        Widgets::RadioButton("Kernel Driver", &bypassMode, 1);
        Widgets::RadioButton("Process Hollowing", &bypassMode, 2);
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Screenshot Bypass");
        static bool screenshotBypass = false;
        Widgets::Toggle("Enable Screenshot Bypass", &screenshotBypass);
        Widgets::FeatureCardEnd();
    }
    else if (s_currentTab == 4) {
        Widgets::FeatureCardStart("Menu Settings");
        static float menuOpacity = 1.0f;
        Widgets::Slider("Menu Opacity", &menuOpacity, 0.1f, 1.0f);
        Widgets::FeatureCardEnd();
        
        Widgets::FeatureCardStart("Keybinds");
        Widgets::Text("Right Shift - Toggle Menu");
        Widgets::Text("Insert - Toggle Menu (backup)");
        Widgets::Text("ESC - Close Menu");
        Widgets::FeatureCardEnd();
    }
    
    Widgets::WindowEnd();
}

void RenderOverlay() {
    Combat::Aimbot::OnRenderOverlay();
    Combat::SilentAim::OnRenderOverlay();
    Combat::Triggerbot::OnRenderOverlay();
    Combat::NoRecoil::OnRenderOverlay();
    Combat::NoSpread::OnRenderOverlay();
    Combat::FovCircle::OnRenderOverlay();
    
    Visual::PlayerESP::OnRenderOverlay();
    Visual::LootESP::OnRenderOverlay();
    Visual::CorpseESP::OnRenderOverlay();
    Visual::Chams::OnRenderOverlay();
    Visual::ArtifactGlow::Render();
    Visual::AnomalyESP::Render();
}

} // namespace GUI
} // namespace VV
