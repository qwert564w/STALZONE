#pragma once
#include <jni.h>
#include <stdint.h>
namespace IL2CPPBridge {
    bool Init();
    jclass FindClass(const char* name);
    jmethodID GetMethod(jclass clazz, const char* name, const char* sig);
    jfieldID GetField(jclass clazz, const char* name, const char* sig);
    uintptr_t PatternScan(uintptr_t base, size_t size, const char* pattern);
}
