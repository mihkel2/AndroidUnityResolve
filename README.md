UnityResolve

A lightweight Unity resolver originally created for modding Counter Critical, now expanded into a full-featured project.

UnityResolve works especially well when paired with Unity wrapper libraries, providing easy access to classes, methods, fields, GameObjects, components, transforms, animations, rendering utilities, and more.

Video Showcase

See UnityResolve in action:

[]()
⸻

Credits

* Sneakyevil — Base API

⸻

Basic Documentation

Classes

Il2CppClass* g_class = nullptr;
// Or resolve a class directly
Il2CppClass* Class = Resolver::GetClass("", "", "");

⸻

Finding Objects

std::vector<void*> objects =
    Resolver::FindObjectsOfType<void*>("");

⸻

Fields

Set Field Value

Resolver::SetFieldValue<T>(
    instance,
    Class,
    "FieldName",
    value
);

⸻

Methods

Resolve Method

Resolver::GetMethod(
    Class,
    "MethodName",
    0
);

Resolve Method Pointer

static uintptr_t addMoneyPtr =
    Resolver::GetMethod(
        Class,
        "AddMoney",
        1 // Argument count
    );

Call Method

Resolver::CallMethod<void>(
    addMoneyPtr,
    instance,
    5000
);

Static methods are resolved and called the same way.

⸻

Example Mod

Il2CppClass* playerClass =
    Resolver::GetClass(
        "",
        "Player",
        "PlayerData"
    );
Resolver::SetFieldValue<int>(
    playerInstance,
    playerClass,
    "coins",
    9999
);

⸻

Features

With the introduction of GameObject, Bone, Mesh, Audio, Animation, and Rendering utilities, UnityResolve has grown far beyond a simple class resolver.

Animation

Get Runtime Animator Controller

Animator_GetRuntimeAnimatorController(
    animator
);

Returns an Il2CppObject* containing the animator’s runtime controller.

Get Animation Clip Names

Animator_GetClipNames(
    animator
);

Returns all clip names associated with the animator.

Child Component Resolution

UnityResolve can also resolve components through child objects and transform hierarchies.

Documentation for this system will be expanded in the future.

⸻

Rendering

World To Screen

WorldToScreen(
    worldPos,
    screenPos,
    matrix
);

Convert a world position into screen coordinates.

Example:

Vector3 screenPos;
if (WorldToScreen(
        playerPosition,
        screenPos,
        viewMatrix))
{
    // Draw ESP
}

⸻

Components

Get Component By Name

il2cppGameObject* component =
    Resolver::GameObject_GetComponentByName(
        gameObject,
        "SkinnedMeshRenderer"
    );

⸻

Camera

Get Main Camera

Camera_GetMain();

⸻

GameObjects

Find GameObject

il2cppObject* player =
    Resolver::GameObject_Find(
        "Player"
    );

⸻

Transforms

Get Transform

void* transform =
    Resolver::Transform_GetTransform(
        player
    );

Get Position

Vector3 position =
    Resolver::Transform_GetPosition(
        transform
    );

Set Local Scale

Resolver::Transform_SetLocalScale(
    transform,
    scale
);

Get Parent

void* parent =
    Resolver::Transform_GetParent(
        transform
    );

