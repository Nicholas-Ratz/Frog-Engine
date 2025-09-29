cmake_minimum_required(VERSION 4.0)
project(FrogEngine)

# =========================
# Set CMake Settings
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# =========================
# Compiler Flags
# =========================
set(CMAKE_CXX_FLAGS "-nostdlib -D_DLL -D_MT")
set(CMAKE_C_FLAGS "-nostdlib -D_DLL -D_MT")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Xclang -gcodeview")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -Xclang -gcodeview")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -flto -DNDEBUG")


# =========================
# Linker Optimizations for Release
# =========================
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -fuse-ld=lld -Wl,/OPT:REF,/OPT:ICF")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -flto -fuse-ld=lld -Wl,/OPT:REF,/OPT:ICF")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE}")
endif()


# =========================
# Files for Library
# =========================
add_library(FrogEngine
        Include/FrogEngine/Window.h
        Include/FrogEngine/Log.h
        Include/FrogEngine/Icon.h
        Include/FrSTD/Utility.h
        Source/FrWindow/Windows/Windows.h
        Source/FrWindow/Windows/Window.cpp
        Source/FrWindow/Windows/KeyInput.cpp
        Source/FrWindow/Windows/TextInput.cpp
        Source/FrWindow/Windows/MouseInput.cpp
        Source/FrLog/Log.cpp
)


# =========================
# Include Directories
# =========================
target_include_directories(FrogEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)


# =========================
# Link with Vulkan
# =========================
find_package(Vulkan REQUIRED)
target_link_libraries(FrogEngine Vulkan::Vulkan)


# =========================
# Files for Example
# =========================
project(Example)
add_executable(Example
        Examples/main.cpp
        Examples/App.rc
    )


# =========================
# Link with Frog-Engine
# =========================
target_link_libraries(Example FrogEngine)


# =========================
# Installing for distribution
# =========================
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/Install" CACHE PATH "Installation directory" FORCE)
install(TARGETS Example DESTINATION .)
install(TARGETS FrogEngine DESTINATION bin)
