#include <jni.h>
#include <string>
#include <thread>
#include "Core/Core.h"
#include "Resolver/Il2CppResolver.h"
#include "Hooks/Hooks.h"
#include "Core/Core.h"


void firstmod() {

    auto instance = nullptr;

   auto klass = Resolver::GetClass("Assembly-CSharp.dll", "", "PlayerController");
    Resolver::SetFieldValue<float>(instance, klass, "walkSpeed", 99.0f);

    auto cls = Resolver::GetClass("Assembly-CSharp.dll", "", "PlayerController");
    Resolver::GetMethod(cls, "",0);


    static auto klass1 = Resolver::GetClass("Assembly-CSharp.dll", "", "PlayerController");
    static uintptr_t addMoneyPtr = Resolver::GetMethod(klass, "AddMoney", 1); // 1 is arg count


    Resolver::CallMethod<void>(addMoneyPtr, instance, 5000);

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
