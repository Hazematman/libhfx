.set noreorder

#include <ucode/fixed_vector.h>

.text
.global _start
_start:
    /* Set stack to end of RAM */
    move $gp, $zero
    addi $sp, $0, 0x1000

    /* Initalize vector register */
    mtc2 zero, $v00, 0
    mtc2 zero, $v00, 1
    mtc2 zero, $v00, 2
    mtc2 zero, $v00, 3
    mtc2 zero, $v00, 4
    mtc2 zero, $v00, 5
    mtc2 zero, $v00, 6
    mtc2 zero, $v00, 7  

    li $2, 2
    mtc2 v0, $v02, 0 
    vaddc $v02, $v00, $v02, w0

    /* Jump to main function */
    jal main
        nop

.data
.global hfx_registers
hfx_registers:
    .fill 32
