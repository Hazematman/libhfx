MEMORY
{
    ROM : ORIGIN = 0x0000, LENGTH = 4k
}

SECTIONS
{
    OVERLAY 0x0000 :
    {
        .text 
        { 
            "crt.o"(.text)
            *(.text*) 
        }

        .data 
        {
            "crt.o"(.data)
            *(.data* .sdata*)
        }
    } > ROM

    .bss : {*(.*bss*)} > ROM

    /DISCARD/ :
    {
        *(.MIPS.abiflags)
    }
}
