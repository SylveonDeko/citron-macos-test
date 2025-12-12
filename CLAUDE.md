# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Citron is a Nintendo homebrew emulator forked from yuzu. It supports Windows, Linux, macOS, and Android platforms. The codebase is C++20 and uses CMake for building.

## Build Commands

### Desktop (macOS/Linux)
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

### Desktop with specific options
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_QT=ON \                    # Qt frontend (default ON)
    -DENABLE_SDL2=ON \                  # SDL2 frontend (default ON)
    -DCITRON_TESTS=ON \                 # Build tests
    -DUSE_DISCORD_PRESENCE=OFF          # Discord Rich Presence
```

### Windows (MSVC)
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Running Tests
```bash
cd build
ctest
# Or run directly:
./bin/tests
```

### Code Formatting
```bash
# Uses clang-format-20
cmake --build . --target clang-format
```

## Architecture Overview

### Core Emulation (`src/core/`)
- **`core.h/cpp`**: Main `System` class - central orchestration of emulation
- **`cpu_manager.cpp`**: Manages CPU thread scheduling
- **`core_timing.cpp`**: Event scheduler for timed operations
- **`memory.cpp`**: Main memory management and address translation
- **`arm/`**: ARM CPU backends
  - `dynarmic/`: Dynamic recompiler (x86_64 and arm64)
  - `nce/`: Native Code Execution backend (Linux/Android arm64 only)

### HLE Services (`src/core/hle/`)
- **`kernel/`**: Horizon OS kernel implementation (processes, threads, synchronization)
- **`service/`**: Reimplements Nintendo services (am, acc, nvdrv, nfp, hid, etc.)
  - Services follow IPC patterns defined in `cmif_serialization.h`

### Video Core (`src/video_core/`)
- **`gpu.cpp`**: Main GPU class coordinating rendering
- **`renderer_vulkan/`**: Primary Vulkan renderer
- **`renderer_opengl/`**: OpenGL renderer
- **`shader_recompiler/`**: Translates guest shaders to host (SPIR-V/GLSL)
- **`texture_cache/`**: GPU texture management
- **`buffer_cache/`**: GPU buffer management
- **`engines/`**: Maxwell GPU engine implementations (3D, 2D, DMA, compute)

### Audio Core (`src/audio_core/`)
Audio rendering and output management (cubeb, OpenAL backends)

### Frontends
- **`src/citron/`**: Qt6 GUI application
- **`src/citron_cmd/`**: SDL2 command-line application
- **`src/android/`**: Android application (JNI in `app/src/main/jni/`)

### Shader Recompiler (`src/shader_recompiler/`)
- **`frontend/`**: Parses guest shader binaries
- **`ir_opt/`**: Intermediate representation and optimization passes
- **`backend/`**: Generates host shaders (SPIR-V for Vulkan, GLSL for OpenGL)

### Supporting Libraries (`src/`)
- **`common/`**: Utility code (logging, threading, containers, math)
- **`input_common/`**: Controller and input handling
- **`hid_core/`**: HID device emulation
- **`network/`**: Local multiplayer networking

## Code Style

- 4 spaces for indentation (no tabs)
- 100 character column limit
- Braces on same line (Attach style)
- Pointer alignment left (`int* ptr`)
- Includes sorted: system headers, then project headers
- See `src/.clang-format` for full configuration

## Key Compile Definitions

- `USE_DYNARMIC`: Enables dynarmic JIT (x86_64/arm64)
- `HAS_NCE`: Enables Native Code Execution (Linux/Android arm64)
- `ARCHITECTURE_x86_64`, `ARCHITECTURE_arm64`: Target architecture
- `CITRON_UNIX`: Unix-like platform

## External Dependencies

Major dependencies in `externals/`:
- **dynarmic**: ARM dynamic recompiler
- **SDL**: Window/input for citron-cmd
- **cubeb**: Audio output
- **FFmpeg**: Video decoding
- **Vulkan-Headers/VulkanMemoryAllocator**: Vulkan support
- **fmt**: String formatting
- **boost**: Various utilities (context, asio)
- **nlohmann_json**: JSON parsing
- **mbedtls**: Cryptography
