# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr Default Build Configuration

message(STATUS "Loading Minimal Zephyr framework...")

# Set project binary directory
if(NOT DEFINED APPLICATION_BINARY_DIR)
  set(APPLICATION_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "Application Binary Directory")
endif()

set(PROJECT_BINARY_DIR ${APPLICATION_BINARY_DIR})

# Set board configuration
if(NOT DEFINED BOARD)
  set(BOARD mps2_an385)
  message(STATUS "Board not specified, defaulting to ${BOARD}")
endif()

message(STATUS "Selected board: ${BOARD}")

# Include board configuration first (sets ARCH and other board-specific vars)
include(${MINIMAL_ZEPHYR_BASE}/boards/${BOARD}/board.cmake)

# Include architecture-specific toolchain configuration
if(NOT DEFINED ARCH)
  message(FATAL_ERROR "Board ${BOARD} did not define ARCH")
endif()

message(STATUS "Target architecture: ${ARCH}")

# Include the appropriate toolchain file based on architecture
if(ARCH STREQUAL "arm")
  include(${MINIMAL_ZEPHYR_BASE}/cmake/toolchains/arm.cmake)
elseif(ARCH STREQUAL "riscv")
  include(${MINIMAL_ZEPHYR_BASE}/cmake/toolchains/riscv.cmake)
else()
  message(FATAL_ERROR "Unsupported architecture: ${ARCH}")
endif()

# Include linker configuration
include(minimal_linker)

# Function to create the app target
function(minimal_zephyr_app)
  # Get source files from arguments
  set(app_sources ${ARGN})
  
  # Create the app library
  add_library(app STATIC)
  
  # Add sources to app
  if(app_sources)
    target_sources(app PRIVATE ${app_sources})
  endif()
  
  # Set include directories
  target_include_directories(app PUBLIC
    ${MINIMAL_ZEPHYR_BASE}/include
    ${APPLICATION_BINARY_DIR}/include
  )
  
  # Add architecture-specific sources
  if(ARCH STREQUAL "arm")
    target_sources(app PRIVATE
      ${MINIMAL_ZEPHYR_BASE}/arch/arm/core/vector_table.c
      ${MINIMAL_ZEPHYR_BASE}/arch/arm/core/system_clock.c
      ${MINIMAL_ZEPHYR_BASE}/drivers/console.c
    )
    set(STARTUP_SOURCE ${MINIMAL_ZEPHYR_BASE}/arch/arm/core/startup.c)
  elseif(ARCH STREQUAL "riscv")
    target_sources(app PRIVATE
      ${MINIMAL_ZEPHYR_BASE}/arch/riscv/core/startup.c
      ${MINIMAL_ZEPHYR_BASE}/drivers/console.c
    )
    set(STARTUP_SOURCE ${MINIMAL_ZEPHYR_BASE}/arch/riscv/core/startup.c)
  endif()
  
  # Create the final ELF - use startup.c as the required source
  # All other code comes from the linked app library
  add_executable(zephyr.elf ${STARTUP_SOURCE})
  
  # Link app library (which contains all other sources including main)
  target_link_libraries(zephyr.elf PRIVATE app)
  
  # Set include directories
  target_include_directories(zephyr.elf PRIVATE
    ${MINIMAL_ZEPHYR_BASE}/include
    ${APPLICATION_BINARY_DIR}/include
  )
  
  # Configure linker script and flags
  if(ARCH STREQUAL "riscv")
    # RISC-V uses -nostdlib since we don't have newlib installed
    set_target_properties(zephyr.elf PROPERTIES
      LINK_FLAGS "-nostdlib -T${MINIMAL_ZEPHYR_BASE}/boards/${BOARD}/linker.ld ${LINKER_FLAGS} -Wl,--gc-sections -Wl,-Map=${APPLICATION_BINARY_DIR}/zephyr.map"
    )
  else()
    set_target_properties(zephyr.elf PROPERTIES
      LINK_FLAGS "-T${MINIMAL_ZEPHYR_BASE}/boards/${BOARD}/linker.ld ${LINKER_FLAGS} -Wl,--gc-sections -Wl,-Map=${APPLICATION_BINARY_DIR}/zephyr.map"
    )
  endif()
  
  # Set target properties
  set_target_properties(zephyr.elf PROPERTIES
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY ${APPLICATION_BINARY_DIR}
  )
  
  # Add QEMU run target if defined
  if(DEFINED QEMU_COMMAND)
    add_custom_target(qemu
      COMMAND ${QEMU_COMMAND} -kernel $<TARGET_FILE:zephyr.elf>
      DEPENDS zephyr.elf
      COMMENT "Running QEMU for ${BOARD}"
      USES_TERMINAL
    )
  endif()
  
  message(STATUS "Minimal Zephyr app configured for board: ${BOARD}")
endfunction()

# Convenience macro for target_sources on app
define_property(TARGET PROPERTY ZEPHYR_APP
  BRIEF_DOCS "Zephyr app target"
  FULL_DOCS "Target for the Zephyr application"
)

set_property(GLOBAL PROPERTY ZEPHYR_APP app)
