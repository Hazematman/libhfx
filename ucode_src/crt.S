.set noreorder

.text
.global _start
_start:
    /* Set stack to end of RAM */
    move $gp, $zero
    addi $sp, $0, 0x1000
    jal main
        nop

.data
.global hfx_registers
hfx_registers:
    .fill 32