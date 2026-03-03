# SPDX-License-Identifier: Apache-2.0
# Minimal Application CMakeLists.txt
# This demonstrates how to use find_package with the Minimal Zephyr framework

cmake_minimum_required(VERSION 3.20.0)

# Set the search path for MinimalZephyr before find_package
if(DEFINED ENV{MINIMAL_ZEPHYR_BASE})
  set(MINIMAL_ZEPHYR_BASE $ENV{MINIMAL_ZEPHYR_BASE})
endif()

if(NOT DEFINED MINIMAL_ZEPHYR_BASE)
  message(FATAL_ERROR "MINIMAL_ZEPHYR_BASE not set. Please set it to the path of minimal_zephyr directory.")
endif()

# Set the directory containing MinimalZephyrConfig.cmake
set(MinimalZephyr_DIR ${MINIMAL_ZEPHYR_BASE}/share/minimal-zephyr/cmake)

# Find the Minimal Zephyr framework
find_package(MinimalZephyr REQUIRED)

# Define the project
project(hello_minimal)

# Call the app setup function which creates the targets
minimal_zephyr_app(
  src/main.c
)

# The minimal_zephyr_app() function handles creating the app library and zephyr.elf executable
