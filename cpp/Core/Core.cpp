#include "Core/Core.h"
#include "Utils/Logger.h"
#include "Resolver/Il2CppResolver.h"
#include "Hooks/Hooks.h"
#include <thread>
#include <chrono>

namespace Internal {

    void Initialize() {
        LOGI("Module thread started");

        // Wait for libil2cpp.so to load
        while (MemoryUtils::get_base_address("libil2cpp.so") == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        if (Resolver::Init()) {
            InitialSetup();
            Hooks::Install();
        }
    }

    void InitialSetup() {
        LOGI("Running User-Friendly Framework Initial Setup...");

        // This is where you can pre-resolve things or check game version
        Il2CppClass* gameMgr = Resolver::GetClass("Assembly-CSharp.dll", "", "GameManager");
        if (gameMgr) {
            LOGI("Found GameManager, template is working perfectly!");
        }

        LOGI("Setup complete. Enjoy modding!");
    }
}
