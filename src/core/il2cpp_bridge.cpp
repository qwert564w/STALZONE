#include "il2cpp_bridge.h"
#include <windows.h>
#include <vector>
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
uintptr_t IL2CPPBridge::PatternScan(uintptr_t base, size_t size, const char* pattern) { return 0; }
