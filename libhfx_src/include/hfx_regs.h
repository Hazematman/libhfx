#ifndef HFX_REGS_H
#define HFX_REGS_H
#include "hfx_cmds.h"

#define HFX_VADDR_RSP_DMEM      0x04000000
#define HFX_VADDR_REG_STATUS    (HFX_VADDR_RSP_DMEM|HFX_REG_STATUS)
#define HFX_VADDR_REG_RB_START  (HFX_VADDR_RSP_DMEM|HFX_REG_RB_START)
#define HFX_VADDR_REG_RB_END    (HFX_VADDR_RSP_DMEM|HFX_REG_RB_END)
#define HFX_VADDR_REG_BAD_OP    (HFX_VADDR_RSP_DMEM|HFX_REG_BAD_OP)


#define HFX_VADDR_REG_RSP_PC    (HFX_VADDR_RSP_DMEM|0x80000)
#define HFX_VADDR_REG_RSP_STATUS (HFX_VADDR_RSP_DMEM|0x40010)

#define HFX_RSP_CLEAR_HALT      0x00000001
#define HFX_RSP_SET_HALT        0x00000002
#define HFX_RSP_CLEAR_BROKE     0x00000004

#endif