#pragma once
#include <stdint.h>
#include <string>

// Compile-time key generation
constexpr uintptr_t OBF_KEY = (__COUNTER__ * 0x5A17B9D3) ^ (__TIME__[0] + __TIME__[1] * 0x100);

constexpr uintptr_t OBFUSCATE_OFFSET(uintptr_t offset) {
    return offset ^ OBF_KEY;
}

constexpr uintptr_t DEOBFUSCATE_OFFSET(uintptr_t offset) {
    return offset ^ OBF_KEY;
}

// Simple macro placeholder for string obfuscation
// Use the python tool for complex arrays
#define OBFUSCATE_STR(s) (s) 
