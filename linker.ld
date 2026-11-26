MEMORY {
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 24K
}

ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS{
    .isr_vector : {
        KEEP(*(.isr_vector))
    } > FLASH

    /*
    .text: function code
    .rodata: const variables and string literals
    */
    .text : {
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    _sidata = LOADADDR(.data); 

    /*
    .data: non const globals and static variables that have nonzero init vals 
    */
    .data : {
        . = ALIGN(4); 
        _sdata = .; 
        *(.data*)
        . = ALIGN(4); 
        _edata = .; 
    } > RAM AT > FLASH

    /*  
    bss: the same as .data but 0 or uninitialized
    */
    .bss : {
        . = ALIGN(4); 
        _sbss = .; 
        *(.bss*)
        *(COMMON)
        . = ALIGN(4); 
        _ebss = .; 
    } > RAM
}