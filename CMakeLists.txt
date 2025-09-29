cmake_minimum_required(VERSION 4.0)
project(FrogEngine)

# =========================
# Set CMake Settings
# =========================
set(CMAKE_CXX_STANDARD 20)


# =========================
# Files for Library
# =========================
add_library(FrogEngine
        Include/FrogEngine/Window.h
        Include/FrogEngine/Log.h
        Include/FrSTD/Utility.h
        Source/FrWindow/Windows/Windows.h
        Source/FrWindow/Windows/Window.cpp
        Source/FrWindow/Windows/KeyInput.cpp
        Source/FrWindow/Windows/TextInput.cpp
        Source/FrWindow/Windows/MouseInput.cpp
        Source/FrLog/Log.cpp
        Include/FrogEngine/Icon.h
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


project(Example)
add_executable(Example
        Examples/main.cpp
        Examples/app.rc)
target_link_libraries(Example FrogEngine)
