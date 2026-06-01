cmake_minimum_required(VERSION 3.22.1)

project("myapplication")

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(ThirdParty)
include_directories(ThirdParty/Dobby/include)

# Source files
set(SOURCE_FILES
    main.cpp
    Core/Core.cpp
    Resolver/Il2CppResolver.cpp
    Hooks/Hooks.cpp
)

# Setup Dobby Library
add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Dobby/lib/${ANDROID_ABI}/libdobby.a)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCE_FILES})

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
    dobby
)
