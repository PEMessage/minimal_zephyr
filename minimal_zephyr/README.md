# Minimal Zephyr Build Instructions

## Project Structure

```
minimal_zephyr/           # Framework project (like Zephyr RTOS)
├── CMakeLists.txt        # Main framework CMake
├── cmake/
│   └── modules/
│       ├── minimal_default.cmake    # Main build configuration
│       ├── minimal_linker.cmake     # Linker setup
│       ├── minimal_package.cmake    # Package export
│       └── minimal_toolchain.cmake  # Toolchain configuration
├── share/minimal-zephyr/cmake/
│   └── MinimalZephyrConfig.cmake    # find_package support
├── boards/
│   └── mps2_an385/
│       ├── board.cmake              # Board configuration
│       └── linker.ld                # Linker script
├── arch/arm/core/
│   ├── vector_table.c               # Interrupt vector table
│   └── startup.c                    # Startup code
├── include/
│   ├── minimal_zephyr.h             # Main header
│   └── arch/arm/
│       ├── cortex_m/cmsis.h         # CMSIS headers
│       └── drivers/uart.h           # UART driver
└── src/
    └── main.c                       # Default main

minimal_app/               # Application project
├── CMakeLists.txt         # App CMake using find_package
└── src/
    └── main.c             # App source
```

## Building

### 1. Set Environment Variable (Optional)
```bash
export MINIMAL_ZEPHYR_BASE=/path/to/minimal_zephyr
```

### 2. Build the Application
```bash
cd minimal_app
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$MINIMAL_ZEPHYR_BASE/cmake/modules/minimal_toolchain.cmake
make
```

Or without environment variable:
```bash
cd minimal_app
mkdir build && cd build
cmake .. -DMINIMAL_ZEPHYR_BASE=/path/to/minimal_zephyr
make
```

### 3. Run with QEMU

Use semihosting for output:

```bash
qemu-system-arm -cpu cortex-m3 -machine mps2-an385 -nographic -m 4M \
  -kernel zephyr.elf.elf -semihosting
```

Expected output:
```
========================================
  Minimal Zephyr App Started!
  This is a custom application.
========================================

Custom app running... Count: 0
Custom app running... Count: 1
Custom app running... Count: 2
...
```

## Key Features

- **CMake Package System**: Uses `find_package(MinimalZephyr)` like real Zephyr
- **Board Support**: Configurable via BOARD variable (default: mps2_an385)
- **Toolchain**: Cross-compilation for ARM Cortex-M3
- **Linker Script**: Proper memory layout for MPS2 AN385 (4MB FLASH, 4MB SRAM)
- **Startup Code**: Full vector table and reset handler
- **Semihosting**: Debug output via ARM semihosting (works with QEMU)
- **QEMU Support**: Ready for emulation

## File Tree

```
minimal_zephyr/           # Framework project (like Zephyr RTOS)
├── CMakeLists.txt        # Main framework CMake
├── cmake/
│   └── modules/
│       ├── minimal_default.cmake    # Main build configuration
│       ├── minimal_linker.cmake     # Linker setup
│       ├── minimal_package.cmake    # Package export
│       └── minimal_toolchain.cmake  # Toolchain configuration
├── share/minimal-zephyr/cmake/
│   └── MinimalZephyrConfig.cmake    # find_package support
├── boards/
│   └── mps2_an385/
│       ├── board.cmake              # Board configuration
│       └── linker.ld                # Linker script
├── arch/arm/core/
│   ├── vector_table.c               # Interrupt vector table
│   ├── startup.c                    # Startup code
│   └── system_clock.c               # System clock
├── include/
│   ├── minimal_zephyr.h             # Main header
│   └── arch/arm/
│       ├── cortex_m/cmsis.h         # CMSIS headers
│       ├── drivers/uart.h           # UART driver
│       └── semihosting.h            # Semihosting for QEMU
└── src/
    └── main.c                       # Default main

minimal_app/               # Application project
├── CMakeLists.txt         # App CMake using find_package
└── src/
    └── main.c             # App source
```

## Mimicking Zephyr Build System

This minimal framework demonstrates:

1. **Package Config**: `MinimalZephyrConfig.cmake` found via `find_package()`
2. **Boilerplate**: `minimal_default.cmake` sets up the build
3. **Toolchain**: Automatic ARM cross-compiler detection
4. **Board System**: Modular board configuration in `boards/<board>/`
5. **Linker Generation**: Custom linker script for target memory layout
6. **app Target**: Library target that applications link against

Similar to real Zephyr's:
- `find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})`
- `project(hello_world)`
- `target_sources(app PRIVATE src/main.c)`
