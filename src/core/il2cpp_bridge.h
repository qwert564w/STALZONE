#pragma once
#include <windows.h>
#include <jni.h>
#include <string>
#include <vector>

namespace IL2CPP {
    extern JavaVM* g_jvm;
    extern JNIEnv* g_env;
    extern jvmtiEnv* g_jvmti;
    bool Init();
    void Shutdown();
    jclass FindClass(const char* name);
    jmethodID GetMethod(jclass cls, const char* name, const char* sig);
    jfieldID  GetField(jclass cls, const char* name, const char* sig);
    template<typename T> T ReadField(jobject obj, jfieldID field);
    template<typename T> void WriteField(jobject obj, jfieldID field, T value);
}