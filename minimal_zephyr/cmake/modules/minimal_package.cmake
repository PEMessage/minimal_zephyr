# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr Package Configuration

# Set up the module path
list(PREPEND CMAKE_MODULE_PATH ${MINIMAL_ZEPHYR_BASE}/cmake/modules)

# Export the package for find_package support
export(PACKAGE MinimalZephyr)
