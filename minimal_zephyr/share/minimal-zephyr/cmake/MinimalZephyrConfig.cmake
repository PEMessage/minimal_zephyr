# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr Config Package
# This file is found by find_package(MinimalZephyr)

# Include boilerplate that sets up the build
macro(include_boilerplate)
  # Set up module path
  list(PREPEND CMAKE_MODULE_PATH ${MINIMAL_ZEPHYR_BASE}/cmake/modules)
  
  # Mark package as found
  set(MinimalZephyr_FOUND True)
  
  # Include the main build configuration
  include(minimal_default)
endmacro()

# Check environment variable
if(DEFINED ENV{MINIMAL_ZEPHYR_BASE})
  set(MINIMAL_ZEPHYR_BASE $ENV{MINIMAL_ZEPHYR_BASE} CACHE PATH "Minimal Zephyr base")
  include_boilerplate()
  return()
endif()

# Check cached variable
if(DEFINED MINIMAL_ZEPHYR_BASE)
  include_boilerplate()
  return()
endif()

# Try to find the package relative to this file
get_filename_component(MINIMAL_ZEPHYR_BASE ${CMAKE_CURRENT_LIST_FILE}/../../.. ABSOLUTE)
set(MINIMAL_ZEPHYR_BASE ${MINIMAL_ZEPHYR_BASE} CACHE PATH "Minimal Zephyr base")
include_boilerplate()
