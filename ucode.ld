SECTIONS
{
    .data 0x0000 : AT (0x04000000)
    {
        "*crt.o"(.data)
        *(.*data*)
        *(.*bss*)
    }

    .text 0x0000 : AT (0x04001000)
    {
        "*crt.o"(.text)
        *(.text*)
    }

    /DISCARD/ :
    {
        *(.MIPS.abiflags)
    }
}
