
The il2cpp resolver I made originally made to mod counter critical and became a full time project :D



works well when paired with unity wrappers



contubition
sneakyevil base api




# Basic coduementation
//find objects ofalltype
std::vector<void*> name = Resolver::FindObjectsOfType<void*>("");

  
    Resolver::SetFieldValue<T>(instance, class, "namespace", float);

   
    Resolver::GetMethod(cls, "",0);


   
    static uintptr_t addMoneyPtr = Resolver::GetMethod(class, "Name", 1); // arg


    Resolver::CallMethod<void>(addMoneyPtr, instance, 5000);

    all of this is the same with the static stuff too

}
