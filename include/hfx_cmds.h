#ifndef HFX_CMDS_H
#define HFX_CMDS_H

#define HFX_REGISTER_SPACE_SIZE 0x0020

#define HFX_REG_STATUS          0x0000
#define     HFX_STATUS_INVALID_OP 0x0001
#define HFX_REG_RB_ADDR         0x0004
#define HFX_REG_RB_SIZE         0x0008
#define HFX_REG_BAD_OP          0x000C
// 0x000C Reserved
#define HFX_REG_RB_START        0x0010
#define HFX_REG_RB_END          0x0014
#define HFX_REG_RDP_START       0x0018
#define HFX_REG_RDP_END         0x001C

#define HFX_CMD_ARG_MASK        0xFF00
#define HFX_CMD_ARG_SHIFT       8
#define HFX_TOTAL_CMDS          0x0020

#define HFX_CMD_NOP             0x0000
#define HFX_CMD_INT             0x0001
#define HFX_CMD_DMA             0x0002
#define     HFX_CMD_DMA_TYPE_READ   (0 << HFX_CMD_ARG_SHIFT)
#define     HFX_CMD_DMA_TYPE_WRITE  (1 << HFX_CMD_ARG_SHIFT)
#define HFX_CMD_SET_DISPLAY     0x0003
#define HFX_CMD_SEND_RDP        0x0004

#endif
