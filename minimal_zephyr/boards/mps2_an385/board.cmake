# SPDX-License-Identifier: Apache-2.0
# Board configuration for MPS2 AN385 (Cortex-M3)

# Set the architecture
set(ARCH arm)

message(STATUS "Configuring for board: MPS2 AN385 (Cortex-M3)")

# Set board-specific defines
set(BOARD_DEFINES
  -DCONFIG_BOARD_MPS2_AN385=1
  -DCONFIG_ARCH_ARM=1
  -DCONFIG_CPU_CORTEX_M3=1
)

# Add compiler definitions
add_compile_definitions(
  CONFIG_BOARD_MPS2_AN385=1
  CONFIG_ARCH_ARM=1
  CONFIG_CPU_CORTEX_M3=1
)

# QEMU configuration for emulation
set(QEMU_COMMAND qemu-system-arm -cpu cortex-m3 -machine mps2-an385 -nographic -m 4M)
