#include <jni.h>
#include <string>
#include <thread>
#include <vector>
#include "Core/Core.h"
#include "Resolver/Il2CppResolver.h"
#include "Hooks/Hooks.h"

void firstmod() {
// this is just an api showcase
    std::vector<void*> enemies = Resolver::FindObjectsOfType<void*>("");


    auto enemyKlass = Resolver::GetClass("Assembly-CSharp.dll", "", "EnemyController");
    if (!enemyKlass) return;


    static uintptr_t addMoneyPtr = Resolver::GetMethod(enemyKlass, "", 1);


    for (void* enemy : enemies) {

        Resolver::SetFieldValue<float>(enemy, enemyKlass, "", 99.0f);


        if (addMoneyPtr) {
            Resolver::CallMethod<void>(addMoneyPtr, enemy, 5000);
        }
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Internal Module Active";
    return env->NewStringUTF(hello.c_str());
}

// JNI_OnLoad is called when the library is loaded
extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Start initialization thread
    std::thread(Internal::Initialize).detach();

    return JNI_VERSION_1_6;
}
