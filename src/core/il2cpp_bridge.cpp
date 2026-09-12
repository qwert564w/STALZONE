#include "il2cpp_bridge.h"
#include <cstdio>
namespace IL2CPP {
JavaVM* g_jvm = nullptr;
JNIEnv* g_env = nullptr;
jvmtiEnv* g_jvmti = nullptr;
bool Init() { return true; }
void Shutdown() {}
jclass FindClass(const char* name) { return nullptr; }
jmethodID GetMethod(jclass cls, const char* name, const char* sig) { return nullptr; }
jfieldID GetField(jclass cls, const char* name, const char* sig) { return nullptr; }
}