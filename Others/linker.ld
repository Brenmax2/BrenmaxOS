ENTRY(kernel_main) 

SECTIONS
{
    . = 0x7E00;

    .text : ALIGN(16) {
        *(.text.entry)  
        *(.text)
    }

    .data : ALIGN(16) { 
        *(.data) 
    }

    .bss : ALIGN(16) { 
        *(.bss)  
        *(COMMON)
    }
}