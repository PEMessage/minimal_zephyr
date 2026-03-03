# SPDX-License-Identifier: Apache-2.0
# Minimal Zephyr Linker Configuration

# Linker flags
set(LINKERFLAGPREFIX -Wl)

# Function to link the final ELF
function(minimal_link_elf target_name)
  set_target_properties(${target_name} PROPERTIES
    LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS}"
  )
endfunction()

# Add custom command to generate binary from ELF
function(minimal_generate_hex target_name)
  add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O ihex $<TARGET_FILE:${target_name}> ${APPLICATION_BINARY_DIR}/zephyr.hex
    COMMENT "Generating zephyr.hex"
  )
endfunction()

function(minimal_generate_bin target_name)
  add_custom_command(TARGET ${target_name} POST_BUILD
    COMMAND ${ARM_OBJCOPY} -O binary $<TARGET_FILE:${target_name}> ${APPLICATION_BINARY_DIR}/zephyr.bin
    COMMENT "Generating zephyr.bin"
  )
endfunction()
