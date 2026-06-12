# drumcore

Header-only C++17 library providing shared drum pattern data structures for [jk.digital](https://jk.digital) VST3 plugins.

## Features

- 10×32 drum pattern grid (10 instruments, 32nd-note resolution)
- Lock-free SPSC circular buffer for real-time pattern exchange
- GM drum mapping with MIDI velocity conversion
- Genre classification and mapping utilities
- Deterministic seeded randomization for reproducible patterns
- Time signature support (4/4, 3/4, 6/8, 7/8)
- RAII denormal protection (FTZ/DAZ on x86, FZ on ARM64)
- Zero runtime dependencies beyond the C++17 standard library

## Public Headers

| Header | Key Types | Purpose |
|--------|-----------|---------|
| `drumcore.h` | — | Umbrella header (includes everything) |
| `drumgrid.h` | `DrumStep`, `DrumBar`, `DrumPatternBuffer` | Pattern grid and lock-free buffer |
| `drummapping.h` | `GMDrumMap` | GM drum note mapping and MIDI velocity |
| `genremapper.h` | `GenreMapper` | Genre enum ↔ string/index/normalized conversion |
| `constants.h` | `Constants::*` | Grid dimensions, tempo, velocity, timing limits |
| `seed.h` | `Seed` | Deterministic splitmix64 PRNG for pattern generation |
| `timesignature.h` | `TimeSignature` | Active steps and beats-per-bar for time signatures |
| `denormalguard.h` | `DenormalGuard` | RAII FTZ/DAZ scope guard for audio processing |
| `lockfreequeue.h` | `LockFreeQueue<T, N>` | Generic SPSC lock-free ring buffer |
| `version.h` | `DRUMCORE_VERSION_*` | Version macros (generated at build time) |

## Quick Start

```cpp
#include <drumcore/drumcore.h>
using namespace JKDigital;

// Create a bar and set a kick on beat 1
DrumBar bar;
bar.getStep(0, 0).velocity = 0.9f;

// Get the GM MIDI note for instrument 0 (kick)
int note = GMDrumMap::getNote(0);  // 36

// Lock-free pattern exchange between threads
DrumPatternBuffer buffer;
buffer.push(bar);

DrumBar received;
if (buffer.pop(received)) {
    // Use received pattern on audio thread
}
```

## Integration

### CMake subdirectory (recommended for sibling projects)

```cmake
add_subdirectory(path/to/drumcore)
target_link_libraries(your_target PRIVATE drumcore)
```

### FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(drumcore
    GIT_REPOSITORY https://github.com/jk-digital/drumcore.git
    GIT_TAG main
)
FetchContent_MakeAvailable(drumcore)
target_link_libraries(your_target PRIVATE drumcore)
```

### find_package (after install)

```cmake
find_package(drumcore REQUIRED)
target_link_libraries(your_target PRIVATE drumcore::drumcore)
```

## Building and Testing

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cd build && ctest --output-on-failure
```

Tests are built automatically when drumcore is the top-level project. When consumed as a subdirectory, tests are skipped.

## Install

```bash
cmake --build build
cmake --install build --prefix /usr/local
```

## License

Apache-2.0. See [LICENSE](LICENSE) for details.
