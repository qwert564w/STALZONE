#pragma once
#include "obfuscation.h"
#include <stdint.h>

namespace Offsets {
    namespace Player {
        constexpr uintptr_t Position = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x120));
        constexpr uintptr_t Health = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x12C));
        constexpr uintptr_t MaxHealth = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x130));
        constexpr uintptr_t TeamID = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x134));
        constexpr uintptr_t IsDead = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x138));
        constexpr uintptr_t Weapon = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x140));
        constexpr uintptr_t ViewAngles = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x1A4));
        
        constexpr auto JNI_Position = OBFUSCATE_STR("Position");
        constexpr auto JNI_Health = OBFUSCATE_STR("Health");
    }
    namespace Client {
        constexpr uintptr_t LocalPlayer = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x01C00000));
        constexpr uintptr_t ViewMatrix = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x01C00040));
        constexpr uintptr_t IsAiming = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x450));
        constexpr uintptr_t IsFiring = DEOBFUSCATE_OFFSET(OBFUSCATE_OFFSET(0x454));
    }
}
