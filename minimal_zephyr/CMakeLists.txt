# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr Framework CMakeLists.txt

cmake_minimum_required(VERSION 3.20.0)

# Set the framework base directory
set(MINIMAL_ZEPHYR_BASE ${CMAKE_CURRENT_LIST_DIR})

# Export the package configuration for find_package support
include(cmake/modules/minimal_package.cmake)
