/* Minimal Zephyr Linker Script for MPS2 AN385 (Cortex-M3)
 * Based on ARM MPS2 memory map
 */

MEMORY
{
    /* Code Flash - 4MB */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 4M
    
    /* SRAM - 4MB */
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 4M
    
    /* Peripheral region */
    PERIPHERALS (rw) : ORIGIN = 0x40000000, LENGTH = 256M
}

/* Stack size */
STACK_SIZE = 0x10000;  /* 64KB stack */

/* Entry point - the reset handler from vector table */
ENTRY(_reset_handler)

SECTIONS
{
    /* Vector table must be at address 0x00000000 */
    .vectors : ALIGN(256)
    {
        _vector_table = .;
        KEEP(*(.vectors))
        KEEP(*(.vectors.*))
    } > FLASH

    /* Text section in FLASH */
    .text : ALIGN(4)
    {
        _text_start = .;
        
        *(.text*)
        *(.rodata*)
        *(.rodata.*)
        
        /* C++ constructors */
        KEEP(*(.init))
        KEEP(*(.fini))
        
        /* ARM exception tables */
        *(.ARM.exidx*)
        
        _text_end = .;
    } > FLASH
    
    /* Data section to be copied from FLASH to SRAM */
    .data : ALIGN(4)
    {
        _data_start = .;
        __data_start__ = .;  /* newlib compatibility */
        *(.data*)
        *(.data.*)
        _data_end = .;
        __data_end__ = .;  /* newlib compatibility */
    } > SRAM AT > FLASH
    _data_load = LOADADDR(.data);
    
    /* BSS section in SRAM */
    .bss (NOLOAD) : ALIGN(4)
    {
        _bss_start = .;
        __bss_start__ = .;  /* newlib compatibility */
        *(.bss*)
        *(.bss.*)
        *(COMMON)
        _bss_end = .;
        __bss_end__ = .;  /* newlib compatibility */
    } > SRAM
    
    /* Stack at the end of SRAM */
    __stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
    
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
