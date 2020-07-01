MEMORY
{
    ROM : ORIGIN = 0x0000, LENGTH = 4k
}

SECTIONS
{
    OVERLAY :
    {
        .text 
        { 
            "${BUILD_DIR}/main.o"(.text)
            *(.text) 
        }
        .data 
        {
            "${BUILD_DIR}/main.o"(.data)
            *(.data) 
        }
    } > ROM
}
