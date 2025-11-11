cmake_minimum_required(VERSION 3.14)
project(FrogEngine LANGUAGES CXX)


# =========================
# Set CMake Settings
# =========================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


# =========================
# Apply optimizations
# =========================
add_compile_options(
    -fno-exceptions
    -fno-rtti
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -Wno-deprecated
    #-D__SHARED__
)
add_link_options(
    -nostdlib
    -nodefaultlibs
    -fuse-ld=lld
)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(
        -O3
        -ffast-math
        -flto=full
        -DNDEBUG
    )
endif()


# =========================
# Files for Library
# =========================
add_library(FrogEngine #SHARED 
    Source/FrAllocator/Allocator.cpp
    Source/FrAllocator/StaticBlock.cpp
    Source/FrLog/Log.cpp
    Source/FrSave/Read.cpp
    Source/FrSave/Save.cpp
    Source/FrSave/Write.cpp
    Source/FrWindow/OSWindows/Window.cpp
    Source/FrWindow/OSWindows/KeyInput.cpp
    Source/FrWindow/OSWindows/TextInput.cpp
    Source/FrWindow/OSWindows/MouseInput.cpp
)


# =========================
# Include Directories
# =========================
target_include_directories(FrogEngine PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
)


# =========================
# Link with Vulkan
# =========================
find_package(Vulkan REQUIRED)
target_link_libraries(FrogEngine Vulkan::Vulkan)


# =========================
# Set compiler defintions
# =========================
target_compile_definitions(FrogEngine PRIVATE FrogEngine_EXPORTS)


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
# Prevent terminal
# =========================
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_options(Example PRIVATE
        -Wl,/SUBSYSTEM:WINDOWS
        -Wl,/ENTRY:mainCRTStartup
    )
endif()


# =========================
# Installing for distribution
# =========================
install(TARGETS Example FrogEngine
    RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/Install"
)
