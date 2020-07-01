#ifndef HFX_H
#define HFX_H

#define HFX_REGISTER_SPACE_SIZE 0x0020

#define HFX_REG_STATUS          0x0000
#define HFX_REG_RB_ADDR         0x0004
#define HFX_REG_RB_SIZE         0x0008
// 0x000C Reserved
#define HFX_REG_RB_START        0x0010
#define HFX_REG_RB_END          0x0014

#define HFX_TOTAL_CMDS          0x0020

#define HFX_CMD_NOP             0x0000
#define HFX_CMD_INT             0x0001
#define HFX_CMD_DMA_READ        0x0002

#endif
