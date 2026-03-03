# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr Toolchain Configuration

# Set the toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Find the ARM cross compiler
find_program(ARM_C_COMPILER arm-none-eabi-gcc
  PATHS /usr/bin /usr/local/bin
  DOC "ARM C Compiler"
)
find_program(ARM_CXX_COMPILER arm-none-eabi-g++
  PATHS /usr/bin /usr/local/bin
  DOC "ARM C++ Compiler"
)
find_program(ARM_ASSEMBLER arm-none-eabi-as
  PATHS /usr/bin /usr/local/bin
  DOC "ARM Assembler"
)
find_program(ARM_LINKER arm-none-eabi-ld
  PATHS /usr/bin /usr/local/bin
  DOC "ARM Linker"
)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy
  PATHS /usr/bin /usr/local/bin
  DOC "ARM Objcopy"
)
find_program(ARM_SIZE arm-none-eabi-size
  PATHS /usr/bin /usr/local/bin
  DOC "ARM Size"
)

if(NOT ARM_C_COMPILER)
  message(FATAL_ERROR "ARM cross compiler not found. Please install gcc-arm-none-eabi.")
endif()

# Set the compilers
set(CMAKE_C_COMPILER ${ARM_C_COMPILER})
set(CMAKE_CXX_COMPILER ${ARM_CXX_COMPILER})
set(CMAKE_ASM_COMPILER ${ARM_C_COMPILER})
set(CMAKE_LINKER ${ARM_LINKER})

# Compiler flags for Cortex-M3
set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb -mabi=aapcs")

# Set compiler flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -std=c99 -Wall -Wextra -g -O0 -ffunction-sections -fdata-sections -nostartfiles")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -std=c++14 -Wall -Wextra -g -O0 -ffunction-sections -fdata-sections -nostartfiles")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -x assembler-with-cpp")

# Disable compiler checks that don't work for cross-compiling
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

message(STATUS "Using ARM toolchain: ${ARM_C_COMPILER}")
