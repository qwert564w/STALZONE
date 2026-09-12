#include "il2cpp_bridge.h"
#include <windows.h>
#include <vector>
#include <string>
JavaVM* g_jvm = nullptr;
JNIEnv* g_env = nullptr;
bool IL2CPPBridge::Init() {
    jint vmCount = 0; JavaVM* vms[1];
    while (JNI_GetCreatedJavaVMs(vms, 1, &vmCount) != JNI_OK || vmCount == 0) Sleep(100);
    g_jvm = vms[0]; g_jvm->AttachCurrentThread((void**)&g_env, nullptr);
    return true;
}
jclass IL2CPPBridge::FindClass(const char* name) { return g_env ? g_env->FindClass(name) : nullptr; }
jmethodID IL2CPPBridge::GetMethod(jclass c, const char* n, const char* s) { return g_env ? g_env->GetMethodID(c, n, s) : nullptr; }
jfieldID IL2CPPBridge::GetField(jclass c, const char* n, const char* s) { return g_env ? g_env->GetFieldID(c, n, s) : nullptr; }
uintptr_t IL2CPPBridge::PatternScan(uintptr_t base, size_t size, const char* pattern) {
    auto patternBytes = [](const char* pattern) {
        std::vector<int> bytes; const char* start = pattern; const char* end = start + strlen(pattern);
        for (const char* current = start; current < end; ++current) {
            if (*current == '?') { ++current; if (*current == '?') ++current; bytes.push_back(-1); }
            else { bytes.push_back(strtoul(current, (char**)&current, 16)); }
        } return bytes;
    };
    std::vector<int> patternBytesVec = patternBytes(pattern);
    uint8_t* scanBytes = reinterpret_cast<uint8_t*>(base);
    for (size_t i = 0; i < size - patternBytesVec.size(); ++i) {
        bool found = true;
        for (size_t j = 0; j < patternBytesVec.size(); ++j) {
            if (patternBytesVec[j] != -1 && scanBytes[i + j] != patternBytesVec[j]) { found = false; break; }
        }
        if (found) return reinterpret_cast<uintptr_t>(&scanBytes[i]);
    }
    return 0;
}
