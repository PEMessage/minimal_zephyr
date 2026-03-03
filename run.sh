#!/bin/bash


cd minimal_app/

cmake -S . -B build -DMINIMAL_ZEPHYR_BASE=$(readlink -f ../minimal_zephyr)
cmake --build build
qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel build/zephyr.elf.elf -semihosting
