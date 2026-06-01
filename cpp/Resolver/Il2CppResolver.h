#ifndef IL2CPP_RESOLVER_H
#define IL2CPP_RESOLVER_H

#include <stdint.h>
#include <dlfcn.h>
#include <vector>
#include "Utils/Logger.h"

// Basic IL2CPP types
typedef struct Il2CppClass Il2CppClass;
typedef struct Il2CppMethod Il2CppMethod;
typedef struct Il2CppObject Il2CppObject;
typedef struct Il2CppDomain Il2CppDomain;
typedef struct Il2CppAssembly Il2CppAssembly;
typedef struct Il2CppImage Il2CppImage;
typedef struct Il2CppField Il2CppField;
typedef struct Il2CppType Il2CppType;
typedef struct Il2CppArray Il2CppArray;

namespace Resolver {
    // Standard Exports
    extern Il2CppDomain* (*il2cpp_domain_get)();
    extern const Il2CppAssembly** (*il2cpp_domain_get_assemblies)(const Il2CppDomain* domain, size_t* size);
    extern const Il2CppImage* (*il2cpp_assembly_get_image)(const Il2CppAssembly* assembly);
    extern Il2CppClass* (*il2cpp_class_from_name)(const Il2CppImage* image, const char* namespaze, const char* name);
    extern const Il2CppMethod* (*il2cpp_class_get_method_from_name)(Il2CppClass* klass, const char* name, int argsCount);
    extern Il2CppField* (*il2cpp_class_get_field_from_name)(Il2CppClass* klass, const char* name);
    extern void (*il2cpp_field_get_value)(void* obj, Il2CppField* field, void* value);
    extern void (*il2cpp_field_set_value)(void* obj, Il2CppField* field, void* value);
    extern void (*il2cpp_field_static_get_value)(Il2CppField* field, void* value);
    extern void (*il2cpp_field_static_set_value)(Il2CppField* field, void* value);
    extern Il2CppObject* (*il2cpp_type_get_object)(const Il2CppType* type);
    extern const Il2CppType* (*il2cpp_class_get_type)(Il2CppClass* klass);
    extern uint32_t (*il2cpp_array_length)(Il2CppArray* array);

    bool Init();


    Il2CppClass* GetClass(const char* assemblyName, const char* namespaze, const char* className);
    uintptr_t GetMethod(Il2CppClass* klass, const char* methodName, int argsCount);
    Il2CppObject* GetSystemType(Il2CppClass* klass);

    template<typename T, typename... Args>
    T CallMethod(uintptr_t methodPtr, void* instance, Args... args) {
        if (!methodPtr) return T();
        return reinterpret_cast<T(*)(void*, Args...)>(methodPtr)(instance, args...);
    }

    template<typename T>
    T GetFieldValue(void* instance, Il2CppClass* klass, const char* fieldName) {
        if (!instance || !klass) return T();
        Il2CppField* field = il2cpp_class_get_field_from_name(klass, fieldName);
        if (!field) return T();
        T val;
        il2cpp_field_get_value(instance, field, &val);
        return val;
    }

    template<typename T>
    void SetFieldValue(void* instance, Il2CppClass* klass, const char* fieldName, T value) {
        if (!instance || !klass) return;
        Il2CppField* field = il2cpp_class_get_field_from_name(klass, fieldName);
        if (!field) return;
        il2cpp_field_set_value(instance, field, &value);
    }

    template<typename T>
    T GetStaticFieldValue(Il2CppClass* klass, const char* fieldName) {
        if (!klass) return T();
        Il2CppField* field = il2cpp_class_get_field_from_name(klass, fieldName);
        if (!field) return T();
        T val;
        il2cpp_field_static_get_value(field, &val);
        return val;
    }

    template<typename T>
    void SetStaticFieldValue(Il2CppClass* klass, const char* fieldName, T value) {
        if (!klass) return;
        Il2CppField* field = il2cpp_class_get_field_from_name(klass, fieldName);
        if (!field) return;
        il2cpp_field_static_set_value(field, &value);
    }


    template<typename T>
    std::vector<T> FindObjectsOfType(const char* className) {
        std::vector<T> results;

        static auto objKlass = GetClass("UnityEngine.CoreModule.dll", "UnityEngine", "Object");
        if (!objKlass) objKlass = GetClass("UnityEngine.dll", "UnityEngine", "Object");

        static uintptr_t findMethod = GetMethod(objKlass, "FindObjectsOfType", 2);

        auto targetKlass = GetClass("Assembly-CSharp.dll", "", className);
        if (!targetKlass || !findMethod) return results;

        Il2CppObject* typeObj = GetSystemType(targetKlass);
        auto array = CallMethod<Il2CppArray*>(findMethod, nullptr, typeObj, true);

        if (array) {
            uint32_t len = il2cpp_array_length(array);
            for (uint32_t i = 0; i < len; i++) {

                results.push_back(((T*)((uintptr_t)array + 0x20))[i]);
            }
        }
        return results;
    }
}

#endif // IL2CPP_RESOLVER_H
