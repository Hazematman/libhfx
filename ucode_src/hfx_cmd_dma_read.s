#include <ucode.S>
#include <ucode/rsp.h>
#include <ucode/hfx_asm.h>

.text
.global hfx_cmd_dma_read
hfx_cmd_dma_read:
    addi $t4, REG_RB_START, 4
    andi $t4, $t4, 0x3FFF // TODO replace this with properly calculated bit mask
    lw $t5, %lo(hfx_rb_buffer)($t4)
    addi $t4, $t4, 4
    andi $t4, $t4, 0x3FFF // TODO replace this with properly calculated bit mask
    lw $t6, %lo(hfx_rb_buffer)($t4)
    addi $t4, $t4, 4
    andi $t4, $t4, 0x3FFF // TODO replace this with properly calculated bit mask
    lw $t7, %lo(hfx_rb_buffer)($t4)
    mtc0 $t5, $c0
    mtc0 $t6, $c1
    mtc0 $t7, $c2
hfx_cmd_dma_read_wait:
    mfc0 $t5, $c6 // Check DMA busy status
    bne $t5, $0, hfx_cmd_dma_read_wait
        nop
    jr $ra
        addi REG_RB_START, REG_RB_START, 16 // TODO adjust the add with how many commands we read
