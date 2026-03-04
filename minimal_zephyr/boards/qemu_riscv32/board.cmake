# SPDX-License-Identifier: Apache-2.0
# Board configuration for QEMU RISC-V 32-bit

# Set architecture
set(ARCH riscv)

message(STATUS "Configuring for board: QEMU RISC-V 32-bit (virt)")

# Set board-specific defines
set(BOARD_DEFINES
  -DCONFIG_BOARD_QEMU_RISCV32=1
  -DCONFIG_ARCH_RISCV=1
  -DCONFIG_ARCH_RISCV32=1
)

# Add compiler definitions
add_compile_definitions(
  CONFIG_BOARD_QEMU_RISCV32=1
  CONFIG_ARCH_RISCV=1
  CONFIG_ARCH_RISCV32=1
)

# QEMU configuration for emulation
set(QEMU_COMMAND qemu-system-riscv32)
set(QEMU_FLAGS
  -machine virt
  -bios none
  -nographic
  -m 256M
)

# QEMU run target
set(QEMU_COMMAND qemu-system-riscv32 ${QEMU_FLAGS})
