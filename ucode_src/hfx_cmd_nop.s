#include <ucode.S>
#include <ucode/rsp.h>
#include <ucode/hfx_asm.h>

.text
.global hfx_cmd_nop
hfx_cmd_nop:
    jr $ra
        addi REG_RB_START, REG_RB_START, 4
