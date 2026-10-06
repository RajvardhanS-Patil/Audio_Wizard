cmake_minimum_required(VERSION 3.15)
project(AudioWizard VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 0)

# Use FetchContent to pull Raylib automatically
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
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

# Test executable
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
if(TEST_SOURCES)
    set(LIB_SOURCES ${SOURCES})
    list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")
    add_executable(AudioWizardTests ${TEST_SOURCES} ${LIB_SOURCES})
    target_include_directories(AudioWizardTests PRIVATE src)
    target_link_libraries(AudioWizardTests PRIVATE raylib)
    set_target_properties(AudioWizardTests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

