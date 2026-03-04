/* Minimal Zephyr Linker Script for QEMU RISC-V 32-bit (virt machine)
 * Based on QEMU virt machine memory map
 */

MEMORY
{
    /* RAM - 128MB at 0x80000000 (QEMU virt machine default) */
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

/* Stack size */
STACK_SIZE = 0x10000;  /* 64KB stack */

/* Entry point - the reset handler */
ENTRY(_reset_handler)

SECTIONS
{
    /* Text section in RAM */
    .text : ALIGN(4)
    {
        _text_start = .;
        
        /* Entry point code first */
        *(.text._reset_handler)
        *(.text._start)
        
        *(.text*)
        *(.rodata*)
        *(.rodata.*)
        
        /* C++ constructors */
        KEEP(*(.init))
        KEEP(*(.fini))
        
        _text_end = .;
    } > RAM
    
    /* Data section */
    .data : ALIGN(4)
    {
        _data_start = .;
        __data_start__ = .;
        *(.data*)
        *(.data.*)
        _data_end = .;
        __data_end__ = .;
    } > RAM
    _data_load = LOADADDR(.data);
    
    /* BSS section */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss_start = .;
        __bss_start__ = .;
        *(.bss*)
        *(.bss.*)
        *(COMMON)
        _bss_end = .;
        __bss_end__ = .;
    } > RAM
    
    /* Stack at the end of RAM */
    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
    
    /* Heap after BSS */
    _end = .;
    __heap_start = .;
    
    /* Discard unnecessary sections */
    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame)
    }
}
