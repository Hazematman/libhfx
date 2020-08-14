#include <ucode.S>
#include <ucode/rsp.h>
#include <ucode/hfx_asm.h>

.text
.global hfx_rdp_init
hfx_rdp_init:
    /* Instruction RDP to use XBUS DMEM for DMA */
    ori $t0, $zero, HFX_SET_XBUX_DMEM_DMA
    mtc0 $t0, $c11
    /* Set RDP start and end address to start of rdp buffer */
    ori $t0, $zero, %lo(hfx_rdp_buffer)
    mtc0 $t0, $c8
    mtc0 $t0, $c9
    jr $ra
        nop

.global hfx_rdp_reserve
hfx_rdp_reserve:
    ld $t0, %lo(hfx_registers + HFX_REG_RDP_START)($0)
    ld $t1, %lo(hfx_registers + HFX_REG_RDP_END)($0)
    /* calculate RDP RB size */
    sub $t0, $t1, $t0
    jr  $ra
        nop

.global hfx_rdp_submit
hfx_rdp_submit:
    jr $ra
        nop