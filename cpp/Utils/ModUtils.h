#ifndef MOD_UTILS_H
#define MOD_UTILS_H

#include <string>
#include <vector>
#include "Resolver/Il2CppResolver.h"
#include "dobby.h"

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
