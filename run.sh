#!/bin/bash


cd minimal_app/

cmake -S . -B build -DMINIMAL_ZEPHYR_BASE=$(readlink -f ../minimal_zephyr) -DBOARD=mps2_an385
# cmake -S . -B build -DMINIMAL_ZEPHYR_BASE=$(readlink -f ../minimal_zephyr) -DBOARD=qemu_riscv32

cmake --build build
qemu-system-arm -machine mps2-an385 -cpu cortex-m3 -kernel build/zephyr.elf.elf
# qemu-system-riscv32 -machine virt -bios none -kernel zephyr.elf.elf
