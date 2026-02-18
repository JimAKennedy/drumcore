# drumcore

Header-only C++17 library providing shared drum pattern data structures for VST3 plugins.

## Build & Test

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

## Architecture

- **Header-only** INTERFACE library — no .cpp files, no linking required
- Namespace: `JKDigital`
- Headers live in `include/drumcore/`
- Tests use GoogleTest (fetched via CMake FetchContent)

### Key components

| Header | Purpose |
|--------|---------|
| `drumgrid.h` | `DrumStep`, `DrumBar`, `DrumPatternBuffer` — core 10x32 grid structures |
| `drummapping.h` | `GMDrumMap` — GM MIDI note mappings for the 10-instrument grid |
| `constants.h` | Shared constants (grid dimensions, tempo, velocity, timing) |
| `timesignature.h` | `TimeSignature` enum and utility functions |
| `lockfreequeue.h` | Generic SPSC lock-free queue template |
| `drumcore.h` | Convenience header that includes all of the above |

## Conventions

- C++17 standard, no extensions (`CMAKE_CXX_EXTENSIONS OFF`)
- Real-time safe: no allocations or blocking in audio-path structures
- `#pragma once` for include guards
- Copyright header: `Copyright(c) 2025-2026 JK Digital.`
- Constants use `k` prefix in `Constants` namespace, UPPER_CASE in `GMDrumMap`
- Test files: `tests/<name>_test.cpp` — one per header
