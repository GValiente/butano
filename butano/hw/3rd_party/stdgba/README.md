# stdgba

[![CI](https://github.com/felixjones/stdgba/actions/workflows/ci.yml/badge.svg)](https://github.com/felixjones/stdgba/actions/workflows/ci.yml)
[![Docs](https://github.com/felixjones/stdgba/actions/workflows/docs.yml/badge.svg)](https://github.com/felixjones/stdgba/actions/workflows/docs.yml)

`stdgba` is a modern C++23 library for Game Boy Advance development.

It is a low-level toolkit, not a game engine: typed hardware register wrappers, BIOS helpers,
fixed-point and angle math, DMA/video/input APIs, test and benchmark harnesses, compile-time
format/music/embed pipelines, and optimized runtime routines for ARM7TDMI targets.

## Why stdgba

- C++23 APIs designed for real GBA constraints (tight CPU, RAM, and VRAM budgets)
- Header-first public interface in `include/gba/` with optimized `.s`/`.cpp` runtime code
- Strong compile-time tooling (`consteval` parsers/compilers for formats, music, timers, etc.)
- Emulator-friendly test and benchmark workflows for repeatable ROM validation
- Practical abstractions without hiding hardware details

## Quick example

```cpp
#include <gba/bios>
#include <gba/interrupt>
#include <gba/peripherals>

int main() {
    gba::irq_handler = {};
    gba::reg_dispstat = {.enable_irq_vblank = true};
    gba::reg_ie = {.vblank = true};
    gba::reg_ime = true;

    while (true) {
        gba::VBlankIntrWait();
    }
}
```

## Requirements

- CMake 3.27+
- ARM GNU Toolchain (`arm-none-eabi`) for GBA cross-compilation

Current development is validated against ARM GNU Toolchain (`arm-none-eabi`) and `gba-toolchain` CMake toolchain (experimental branch).

## Core modules

- `gba/fixed_point`, `gba/angle`: deterministic arithmetic types for game math
- `gba/video`, `gba/sprite`, `gba/color`, `gba/peripherals`: display and register-layer APIs
- `gba/dma`, `gba/timer`, `gba/interrupt`: hardware control and scheduling primitives
- `gba/testing`, `gba/benchmark`: ROM-native validation and cycle measurement
- `gba/music`, `gba/format`, `gba/embed`: compile-time authoring pipelines
- `gba/save`, `gba/overlay`, `gba/memory`: cartridge persistence and memory helpers

## Repository layout

- `include/gba/` - public headers (extensionless facade headers)
- `include/gba/bits/` - implementation details
- `source/` - optimized assembly and C++ runtime units
- `tests/` - one ROM test target per `test_*.cpp`
- `benchmarks/` - performance comparisons and measurement harnesses

## CI and documentation

- CI (`.github/workflows/ci.yml`) builds test ELFs and runs them with `felixjones/github-mgba-rom-test`.
- The Book (`book/`) contains detailed documentation and examples, built with `mdBook` and published to [felixjones.github.io/stdgba](https://felixjones.github.io/stdgba).

## License

`stdgba` is distributed under the zlib license. See `LICENSE`.
