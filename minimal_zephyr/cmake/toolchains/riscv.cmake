# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr RISC-V Toolchain Configuration

# Set the system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

# Find the RISC-V cross compiler
find_program(RISCV_C_COMPILER riscv64-unknown-elf-gcc
  PATHS /usr/bin /usr/local/bin /opt/riscv/bin
  DOC "RISC-V C Compiler"
)
find_program(RISCV_CXX_COMPILER riscv64-unknown-elf-g++
  PATHS /usr/bin /usr/local/bin /opt/riscv/bin
  DOC "RISC-V C++ Compiler"
)
find_program(RISCV_ASSEMBLER riscv64-unknown-elf-as
  PATHS /usr/bin /usr/local/bin /opt/riscv/bin
  DOC "RISC-V Assembler"
)
find_program(RISCV_LINKER riscv64-unknown-elf-ld
  PATHS /usr/bin /usr/local/bin /opt/riscv/bin
  DOC "RISC-V Linker"
)
find_program(RISCV_OBJCOPY riscv64-unknown-elf-objcopy
  PATHS /usr/bin /usr/local/bin /opt/riscv/bin
  DOC "RISC-V Objcopy"
)
find_program(RISCV_SIZE riscv64-unknown-elf-size
  PATHS /usr/bin /usr/local/bin /opt/riscv/bin
  DOC "RISC-V Size"
)

# Also try 32-bit specific names
if(NOT RISCV_C_COMPILER)
  find_program(RISCV_C_COMPILER riscv32-unknown-elf-gcc
    PATHS /usr/bin /usr/local/bin /opt/riscv/bin
    DOC "RISC-V C Compiler"
  )
  find_program(RISCV_CXX_COMPILER riscv32-unknown-elf-g++
    PATHS /usr/bin /usr/local/bin /opt/riscv/bin
    DOC "RISC-V C++ Compiler"
  )
  find_program(RISCV_ASSEMBLER riscv32-unknown-elf-as
    PATHS /usr/bin /usr/local/bin /opt/riscv/bin
    DOC "RISC-V Assembler"
  )
  find_program(RISCV_LINKER riscv32-unknown-elf-ld
    PATHS /usr/bin /usr/local/bin /opt/riscv/bin
    DOC "RISC-V Linker"
  )
  find_program(RISCV_OBJCOPY riscv32-unknown-elf-objcopy
    PATHS /usr/bin /usr/local/bin /opt/riscv/bin
    DOC "RISC-V Objcopy"
  )
  find_program(RISCV_SIZE riscv32-unknown-elf-size
    PATHS /usr/bin /usr/local/bin /opt/riscv/bin
    DOC "RISC-V Size"
  )
endif()

if(NOT RISCV_C_COMPILER)
  message(FATAL_ERROR "RISC-V cross compiler not found. Please install riscv-gnu-toolchain.")
endif()

# Set the compilers
set(CMAKE_C_COMPILER ${RISCV_C_COMPILER})
set(CMAKE_CXX_COMPILER ${RISCV_CXX_COMPILER})
set(CMAKE_ASM_COMPILER ${RISCV_C_COMPILER})
set(CMAKE_LINKER ${RISCV_LINKER})

# Compiler flags for RISC-V 32-bit
# -march=rv32gc: RV32I base with M, A, F, D, C extensions
# -mabi=ilp32: Integer, Long, Pointer 32-bit (use ilp32d if FPU is available)
set(MCU_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medany")

# Set compiler flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -std=c99 -Wall -Wextra -g -O0 -ffunction-sections -fdata-sections -nostartfiles -ffreestanding")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -std=c++14 -Wall -Wextra -g -O0 -ffunction-sections -fdata-sections -nostartfiles -ffreestanding")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -x assembler-with-cpp")

# Linker flags for RISC-V
set(LINKER_FLAGS "")

# Disable compiler checks that don't work for cross-compiling
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

message(STATUS "Using RISC-V toolchain: ${RISCV_C_COMPILER}")
