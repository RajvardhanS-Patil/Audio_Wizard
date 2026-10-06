# Build and Run Instructions

## Prerequisites

- C++17 Compatible Compiler (MSVC / GCC / Clang)
- CMake 3.15+

## Building

### Linux / macOS
```bash
cmake -B build -S .
cmake --build build -j$(nproc)
```

### Windows (MSVC)
```cmd
cmake -B build -S .
cmake --build build --config Debug
```

Raylib is fetched and compiled automatically via CMake `FetchContent`.

## Running the Game

### Linux / macOS
```bash
./build/AudioWizard
```

### Windows
```cmd
.\build\Debug\AudioWizard.exe
```

## Running Unit Tests

### Linux / macOS
```bash
./build/AudioWizardTests
```

### Windows
```cmd
.\build\Debug\AudioWizardTests.exe
```