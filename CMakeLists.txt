cmake_minimum_required(VERSION 3.15)
project(AudioWizard VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use FetchContent to pull Raylib automatically
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master # Or a specific release tag like 5.0
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define executable
add_executable(AudioWizard ${SOURCES})

# Include directories
target_include_directories(AudioWizard PRIVATE src)

# Link Raylib
target_link_libraries(AudioWizard PRIVATE raylib)

# Optional: Ensure the working directory is set correctly in Visual Studio / other IDEs
set_target_properties(AudioWizard PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
