#ifndef MOD_UTILS_H
#define MOD_UTILS_H

#include <string>
#include <vector>
#include "Resolver/Il2CppResolver.h"
#include "dobby.h"

// --- THE "EASY HOOK" MACROS ---
// These are optional. You can also use DobbyHook directly.

#define DEFINE_HOOK(ret, name, ...) \
    ret (*orig_##name)(__VA_ARGS__); \
    ret hook_##name(__VA_ARGS__)

#define INSTALL_HOOK(assembly, ns, clazz, method, args, name) \
    { \
        Il2CppClass* _klass = Resolver::GetClass(assembly, ns, clazz); \
        if (_klass) { \
            uintptr_t _target = Resolver::GetMethod(_klass, method, args); \
            if (_target) { \
                DobbyHook((void*)_target, (void*)hook_##name, (void**)&orig_##name); \
                LOGI("Successfully hooked %s::%s", clazz, method); \
            } else { LOGE("Failed to find method %s in %s", method, clazz); } \
        } else { LOGE("Failed to find class %s", clazz); } \
    }

// --- UTILITY FUNCTIONS ---
namespace ModUtils {
    // Example of a raw function you can call anywhere
    inline void SetPlayerSpeed(float speed) {
        static auto klass = Resolver::GetClass("Assembly-CSharp.dll", "", "PlayerController");
        // In a real mod, you'd need the instance. This is just an example.
        // If the player is a singleton:
        // void* instance = Resolver::GetStaticFieldValue<void*>(klass, "Instance");
        // if (instance) Resolver::SetFieldValue<float>(instance, klass, "walkSpeed", speed);
    }
}

// --- UNITY TYPES ---
struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };
struct Color { float r, g, b, a; };

#endif // MOD_UTILS_H
