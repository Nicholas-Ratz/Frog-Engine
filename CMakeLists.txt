cmake_minimum_required(VERSION 4.0)
project(FrogEngine)

# =========================
# Set CMake Settings
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# =========================
# Files for Library
# =========================
add_library(FrogEngine STATIC
        Include/FrogEngine/Log.h
        Include/FrogEngine/Utility.h
        Include/FrogEngine/Window.h
        Source/FrWindow/Windows/Windows.cpp
        Source/FrWindow/Windows/KeyMapping.h
        Source/FrLog/Log.cpp
)


# =========================
# Include Directories
# =========================
target_include_directories(FrogEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)


# =========================
# Link with Vulkan and OpenAL
# =========================
find_package(Vulkan REQUIRED)
target_link_libraries(FrogEngine Vulkan::Vulkan)


# =========================
# Compiler Flags
# =========================
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(FrogEngine PRIVATE
            $<$<CONFIG:Debug>:-O0 -g -fno-omit-frame-pointer -fno-inline -Wall -Wextra>
            $<$<CONFIG:Release>:-O3 -flto -mtune=generic>
    )
    target_link_options(FrogEngine PRIVATE
            $<$<CONFIG:Release>:-flto -fuse-ld=lld>
    )
endif ()

project(Example)
add_executable(Example
        Examples/main.cpp
        Examples/app.rc)
target_link_libraries(Example FrogEngine)