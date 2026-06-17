#ifndef MOD_UTILS_H
#define MOD_UTILS_H

#include <string>
#include <vector>
#include "Resolver/Il2CppResolver.h"
#include "dobby.h"


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





struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };
struct Color { float r, g, b, a; };

#endif
