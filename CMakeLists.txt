cmake_minimum_required(VERSION 4.0)
project(Frog_Engine)


# =========================
# Set CMake Settings
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Files for Library
# =========================
add_library(Frog_Engine STATIC
        Include/FrogEngine/FrogEngine.h
        Source/FrogEngine.cpp
)


# =========================
# Include Directories
# =========================
target_include_directories(Frog_Engine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Private)


# =========================
# Link with Vulkan and OpenAL
# =========================
find_package(Vulkan REQUIRED)
target_link_libraries(Frog_Engine Vulkan::Vulkan)


# =========================
# Compiler Flags
# =========================
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(Frog_Engine PRIVATE
            $<$<CONFIG:Debug>:-O0 -g -fno-omit-frame-pointer -fno-inline -Wall -Wextra>
            $<$<CONFIG:Release>:-O3 -flto -mtune=generic>
    )
    target_link_options(Frog_Engine PRIVATE
            $<$<CONFIG:Release>:-flto -s -fuse-ld=lld>
    )
endif ()