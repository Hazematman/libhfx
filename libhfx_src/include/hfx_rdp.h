#ifndef HFX_RDP_H
#define HFX_RDP_H


#define HFX_RDP_CMD_SHIFT           56ull
#define HFX_RDP_CMD_MASK            (0x3f << HFX_RDP_CMD_SHIFT)
#define HFX_RDP_CMD_RESERVE         0xc0ull
#define HFX_RDP_CMD_SET_FILL_COLOR  0x37ull
#define HFX_RDP_CMD_FILL_RECT       0x36ull
#define     HFX_RDP_CMD_FILL_RECT_ARG_MASK 0xfffull 
#define     HFX_RDP_CMD_FILL_RECT_XL_SHIFT  44ull
#define     HFX_RDP_CMD_FILL_RECT_YL_SHIFT  32ull
#define     HFX_RDP_CMD_FILL_RECT_XH_SHIFT  12ull
#define     HFX_RDP_CMD_FILL_RECT_YH_SHIFT  0ull
#define HFX_RDP_CMD_SET_MODE        0x2full
#define HFX_RPD_CMD_SET_MODE_RESERVED   0xf00000000ull
#define     HFX_RDP_CMD_SET_MODE_ATOMIC_PRIM (1ull << 55ull)
#define     HFX_RDP_CMD_SET_MODE_ONE_CYCLE  (0ull << 52ull)
#define     HFX_RDP_CMD_SET_MODE_TWO_CYCLE  (1ull << 52ull)
#define     HFX_RDP_CMD_SET_MODE_COPY       (2ull << 52ull)
#define     HFX_RDP_CMD_SET_MODE_FILL       (3ull << 52ull)
#define     HFX_RDP_CMD_SET_MODE_PERSP_TEX_EN   (1ull << 51ull)
#define     HFX_RDP_CMD_SET_MODE_DETAIL_TEX_EN  (1ull << 50ull)
#define     HFX_RDP_CMD_SET_MODE_SHARPEN_TEX_EN (1ull << 49ull)
#define     HFX_RDP_CMD_SET_MODE_TEX_LOD_EN     (1ull << 48ull)
#define     HFX_RDP_CMD_SET_MODE_TLUT_EN        (1ull << 47ull)
#define     HFX_RDP_CMD_SET_MODE_TLUT_RGBA      (0ull << 46ull)
#define     HFX_RDP_CMD_SET_MODE_TLUT_IA        (1ull << 46ull)
#define     HFX_RDP_CMD_SET_MODE_SAMPLE_1X1     (0ull << 45ull)
#define     HFX_RDP_CMD_SET_MODE_SAMPLE_2X2     (1ull << 45ull)
#define     HFX_RDP_CMD_SET_MODE_MID_TEXEL      (1ull << 44ull)
#define     HFX_RDP_CMD_SET_MODE_BI_LERP_0      (1ull << 43ull)
#define     HFX_RDP_CMD_SET_MODE_BI_LERP_1      (1ull << 42ull)
#define     HFX_RDP_CMD_SET_MODE_CONVERT_ONE    (1ull << 41ull)
#define     HFX_RDP_CMD_SET_MODE_KEY_EN         (1ull << 40ull)
#define     HFX_RDP_CMD_SET_MODE_RGB_MAGIC_SQUARE   (0ull << 38ull)
#define     HFX_RDP_CMD_SET_MODE_RGB_BAYER_MATRIX   (1ull << 38ull)
#define     HFX_RDP_CMD_SET_MODE_RGB_NOISE          (2ull << 38ull)
#define     HFX_RDP_CMD_SET_MODE_RGB_NO_DITHER      (3ull << 38ull)
#define     HFX_RDP_CMD_SET_MODE_ALPHA_MAGIC_SQUARE   (0ull << 36ull)
#define     HFX_RDP_CMD_SET_MODE_ALPHA_BAYER_MATRIX   (1ull << 36ull)
#define     HFX_RDP_CMD_SET_MODE_ALPHA_NOISE          (2ull << 36ull)
#define     HFX_RDP_CMD_SET_MODE_ALPHA_NO_DITHER      (3ull << 36ull)
/* TODO missing blend operations here */
#define     HFX_RDP_CMD_SET_MODE_FORCE_BLEND        (1ull << 14ull)
#define     HFX_RDP_CMD_SET_MODE_ALPHA_CVG_SELECT   (1ull << 13ull)
#define     HFX_RDP_CMD_SET_MODE_CVG_TIMES_ALPHA    (1ull << 12ull)
/* TODO mising cvg and z-buffer operations here */


#define HFX_RDP_MAKE_CMD(cmd) ((HFX_RDP_CMD_RESERVE|(cmd)) << HFX_RDP_CMD_SHIFT)
#define HFX_RDP_PKT_SET_FILL_COLOR(color) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_FILL_COLOR) | (color))
#define HFX_RDP_PKT_FILL_RECT(xl, yl, xh, yh) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_FILL_RECT) | \
                                              (((xl)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_XL_SHIFT) | \
                                              (((yl)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_YL_SHIFT) | \
                                              (((xh)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_XH_SHIFT) | \
                                              (((yh)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_YH_SHIFT))
#define HFX_RDP_PKT_SET_MODE(modes) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_MODE) | HFX_RPD_CMD_SET_MODE_RESERVED | (modes))

#endif