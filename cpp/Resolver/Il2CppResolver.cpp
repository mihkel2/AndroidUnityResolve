#include "Il2CppResolver.h"
#include <string.h>

namespace Resolver {
    Il2CppDomain* (*il2cpp_domain_get)() = nullptr;
    const Il2CppAssembly** (*il2cpp_domain_get_assemblies)(const Il2CppDomain* domain, size_t* size) = nullptr;
    const Il2CppImage* (*il2cpp_assembly_get_image)(const Il2CppAssembly* assembly) = nullptr;
    Il2CppClass* (*il2cpp_class_from_name)(const Il2CppImage* image, const char* namespaze, const char* name) = nullptr;
    const Il2CppMethod* (*il2cpp_class_get_method_from_name)(Il2CppClass* klass, const char* name, int argsCount) = nullptr;
    Il2CppField* (*il2cpp_class_get_field_from_name)(Il2CppClass* klass, const char* name) = nullptr;
    void (*il2cpp_field_get_value)(void* obj, Il2CppField* field, void* value) = nullptr;
    void (*il2cpp_field_set_value)(void* obj, Il2CppField* field, void* value) = nullptr;
    void (*il2cpp_field_static_get_value)(Il2CppField* field, void* value) = nullptr;
    void (*il2cpp_field_static_set_value)(Il2CppField* field, void* value) = nullptr;
    Il2CppObject* (*il2cpp_type_get_object)(const Il2CppType* type) = nullptr;
    const Il2CppType* (*il2cpp_class_get_type)(Il2CppClass* klass) = nullptr;
    uint32_t (*il2cpp_array_length)(Il2CppArray* array) = nullptr;

    bool Init() {
        void* handle = dlopen("libil2cpp.so", RTLD_LAZY);
        if (!handle) {
            LOGE("Failed to dlopen libil2cpp.so");
            return false;
        }

        il2cpp_domain_get = (Il2CppDomain* (*)())dlsym(handle, "il2cpp_domain_get");
        il2cpp_domain_get_assemblies = (const Il2CppAssembly** (*)(const Il2CppDomain*, size_t*))dlsym(handle, "il2cpp_domain_get_assemblies");
        il2cpp_assembly_get_image = (const Il2CppImage* (*)(const Il2CppAssembly*))dlsym(handle, "il2cpp_assembly_get_image");
        il2cpp_class_from_name = (Il2CppClass* (*)(const Il2CppImage*, const char*, const char*))dlsym(handle, "il2cpp_class_from_name");
        il2cpp_class_get_method_from_name = (const Il2CppMethod* (*)(Il2CppClass*, const char*, int))dlsym(handle, "il2cpp_class_get_method_from_name");
        il2cpp_class_get_field_from_name = (Il2CppField* (*)(Il2CppClass*, const char*))dlsym(handle, "il2cpp_class_get_field_from_name");
        il2cpp_field_get_value = (void (*)(void*, Il2CppField*, void*))dlsym(handle, "il2cpp_field_get_value");
        il2cpp_field_set_value = (void (*)(void*, Il2CppField*, void*))dlsym(handle, "il2cpp_field_set_value");
        il2cpp_field_static_get_value = (void (*)(Il2CppField*, void*))dlsym(handle, "il2cpp_field_static_get_value");
        il2cpp_field_static_set_value = (void (*)(Il2CppField*, void*))dlsym(handle, "il2cpp_field_static_set_value");
        il2cpp_type_get_object = (Il2CppObject* (*)(const Il2CppType*))dlsym(handle, "il2cpp_type_get_object");
        il2cpp_class_get_type = (const Il2CppType* (*)(Il2CppClass*))dlsym(handle, "il2cpp_class_get_type");
        il2cpp_array_length = (uint32_t (*)(Il2CppArray*))dlsym(handle, "il2cpp_array_length");

        if (!il2cpp_domain_get || !il2cpp_type_get_object) {
            LOGE("Failed to find essential il2cpp symbols for FindObjects!");
            return false;
        }

        LOGI("Il2Cpp Resolver Initialized with Advanced Features");
        return true;
    }

    Il2CppClass* GetClass(const char* assemblyName, const char* namespaze, const char* className) {
        Il2CppDomain* domain = il2cpp_domain_get();
        size_t size;
        const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(domain, &size);

        for (size_t i = 0; i < size; ++i) {
            const Il2CppImage* image = il2cpp_assembly_get_image(assemblies[i]);
            Il2CppClass* klass = il2cpp_class_from_name(image, namespaze, className);
            if (klass) return klass;
        }
        return nullptr;
    }

    uintptr_t GetMethod(Il2CppClass* klass, const char* methodName, int argsCount) {
        const Il2CppMethod* method = il2cpp_class_get_method_from_name(klass, methodName, argsCount);
        if (!method) return 0;
        return *(uintptr_t*)method;
    }

    Il2CppObject* GetSystemType(Il2CppClass* klass) {
        if (!klass) return nullptr;
        return il2cpp_type_get_object(il2cpp_class_get_type(klass));
    }
}
