#include <ucode.S>
#include <ucode/rsp.h>
#include <ucode/hfx_asm.h>

.text
.global hfx_rdp_init
hfx_rdp_init:
    jr $ra
        nop

.global hfx_rdp_reserve
hfx_rdp_reserve:
    jr  $ra
        nop

.global hfx_rdp_submit
hfx_rdp_submit:
    jr $ra
        nop