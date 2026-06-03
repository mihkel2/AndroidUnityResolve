IL2Type-resolve

A lightweight IL2CPP resolver orginal made for modding Counter Critical now a full project :D

Works well when paired with Unity wrappers.

Contribution / Credits

* Sneakyevil — Base API

Basic Documentation

Classes
Il2CppClass* g_class = nullptr;
// Or
Il2CppClass* Class = Resolver::GetClass("", "", "");

std::vector<void*> objects = Resolver::FindObjectsOfType<void*>("");


Resolver::SetFieldValue<T>(instance, Class, "FieldName", value);



Resolver::GetMethod(Class, "MethodName", 0);



static uintptr_t addMoneyPtr =
    Resolver::GetMethod(Class, "AddMoney", 1); // argument count


Resolver::CallMethod<void>(addMoneyPtr, instance, 5000);

static methods are the same btw :)

Mod Example

Il2CppClass* playerClass =
    Resolver::GetClass("", "Player", "PlayerData");
Resolver::SetFieldValue<int>(playerInstance, playerClass, "coins", 9999);
