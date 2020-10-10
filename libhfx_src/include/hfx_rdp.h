#ifndef HFX_RDP_H
#define HFX_RDP_H


#define HFX_RDP_CMD_SHIFT           56ULL
#define HFX_RDP_CMD_MASK            (0x3f << HFX_RDP_CMD_SHIFT)
#define HFX_RDP_CMD_RESERVE         0xe0ull
#define HFX_RDP_CMD_SET_FILL_COLOR  0x37ull
#define HFX_RDP_CMD_FILL_RECT       0x36ull
#define     HFX_RDP_CMD_FILL_RECT_ARG_MASK 0xfffull 
#define     HFX_RDP_CMD_FILL_RECT_XL_SHIFT  44ull
#define     HFX_RDP_CMD_FILL_RECT_YL_SHIFT  32ull
#define     HFX_RDP_CMD_FILL_RECT_XH_SHIFT  12ull
#define     HFX_RDP_CMD_FILL_RECT_YH_SHIFT  0ull
#define HFX_RDP_CMD_SET_MODE        0x2full




#define HFX_RDP_MAKE_CMD(cmd) ((HFX_RDP_CMD_RESERVE|(cmd)) << HFX_RDP_CMD_SHIFT)
#define HFX_RDP_PKT_SET_FILL_COLOR(color) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_FILL_COLOR) | (color))
#define HFX_RDP_PKT_FILL_RECT(xl, yl, xh, yh) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_FILL_RECT) | \
                                              (((xl)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_XL_SHIFT) | \
                                              (((yl)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_YL_SHIFT) | \
                                              (((xh)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_XH_SHIFT) | \
                                              (((yh)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_YH_SHIFT))


#endif