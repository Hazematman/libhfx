#include <ucode.S>
#include <ucode/rsp.h>
#include <ucode/hfx_asm.h>

.text
.global hfx_cmd_set_display
hfx_cmd_set_display:
    addi $t4, REG_RB_START, 4
    andi $t4, $t4, 0x3FFF // TODO replace this with properly calculated bit mask
    lw $t5, %lo(hfx_rb_buffer)($t4)
    // TODO need to implement RDP command buffer
    jr $ra
        addi REG_RB_START, REG_RB_START, 8 // TODO adjust the add with how many commands we read
