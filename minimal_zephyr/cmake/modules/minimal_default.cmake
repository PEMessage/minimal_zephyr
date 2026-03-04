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

# Include toolchain configuration
include(minimal_toolchain)

# Include linker configuration
include(minimal_linker)

# Include board configuration
include(${MINIMAL_ZEPHYR_BASE}/boards/${BOARD}/board.cmake)

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
  
  # Add board-specific sources (startup.c is added to zephyr.elf directly)
  target_sources(app PRIVATE
    ${MINIMAL_ZEPHYR_BASE}/arch/arm/core/vector_table.c
    ${MINIMAL_ZEPHYR_BASE}/arch/arm/core/system_clock.c
    ${MINIMAL_ZEPHYR_BASE}/drivers/console.c
  )
  
  # Create the final ELF - use startup.c as the required source
  # All other code comes from the linked app library
  add_executable(zephyr.elf ${MINIMAL_ZEPHYR_BASE}/arch/arm/core/startup.c)
  
  # Link app library (which contains all other sources including main)
  target_link_libraries(zephyr.elf PRIVATE app)
  
  # Set include directories
  target_include_directories(zephyr.elf PRIVATE
    ${MINIMAL_ZEPHYR_BASE}/include
    ${APPLICATION_BINARY_DIR}/include
  )
  
  # Configure linker script and flags
  set_target_properties(zephyr.elf PROPERTIES
    LINK_FLAGS "-T${MINIMAL_ZEPHYR_BASE}/boards/${BOARD}/linker.ld -specs=nano.specs -lc -lm -lnosys -Wl,--gc-sections -Wl,-Map=${APPLICATION_BINARY_DIR}/zephyr.map"
  )
  
  # Set target properties
  set_target_properties(zephyr.elf PROPERTIES
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY ${APPLICATION_BINARY_DIR}
  )
  
  message(STATUS "Minimal Zephyr app configured for board: ${BOARD}")
endfunction()

# Convenience macro for target_sources on app
define_property(TARGET PROPERTY ZEPHYR_APP
  BRIEF_DOCS "Zephyr app target"
  FULL_DOCS "Target for the Zephyr application"
)

set_property(GLOBAL PROPERTY ZEPHYR_APP app)
