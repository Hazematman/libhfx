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
            "${BUILD_DIR}/crt.o"(.text)
            *(.text*) 
        }

        .data 
        {
            "${BUILD_DIR}/crt.o"(.data)
            *(.*data*)
        }
    } > ROM

    .bss : {*(.*bss*)} > ROM

    /DISCARD/ :
    {
        *(.MIPS.abiflags)
    }
}