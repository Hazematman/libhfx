#ifndef HFX_RDP_H
#define HFX_RDP_H


#define HFX_RDP_CMD_SHIFT           56ull
#define HFX_RDP_CMD_MASK            (0x3f << HFX_RDP_CMD_SHIFT)
#define HFX_RDP_CMD_SET_COLOR_IMAGE 0x3full
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_RGBA    0ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_YUV     1ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_INDEX   2ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_IA      3ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_I       4ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_SIZE_4B        0ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_SIZE_8B        1ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_SIZE_16B       2ull
#define     HFX_RDP_CMD_SET_COLOR_IMAGE_SIZE_32B       3ull
#define HFX_RDP_CMD_SET_Z_IMAGE     0x3eull
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
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_MASK         3ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_1A_0_SHIFT   30ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_1A_1_SHIFT   28ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_1B_0_SHIFT   26ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_1B_1_SHIFT   24ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_2A_0_SHIFT   22ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_2A_1_SHIFT   20ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_2B_0_SHIFT   18ull
#define     HFX_RDP_CMD_SET_MODE_BLEND_MODE_2B_1_SHIFT   16ull
#define     HFX_RDP_CMD_SET_MODE_FORCE_BLEND        (1ull << 14ull)
#define     HFX_RDP_CMD_SET_MODE_ALPHA_CVG_SELECT   (1ull << 13ull)
#define     HFX_RDP_CMD_SET_MODE_CVG_TIMES_ALPHA    (1ull << 12ull)
/* TODO mising cvg and z-buffer operations here */
#define     HFX_RDP_CMD_SET_MODE_DEPTH_MODE_OPAQUE              (0ull << 10ull)
#define     HFX_RDP_CMD_SET_MODE_DEPTH_MODE_INTERPENETRATING    (1ull << 10ull)
#define     HFX_RDP_CMD_SET_MODE_DEPTH_MODE_TRANSPARENT         (2ull << 10ull)
#define     HFX_RDP_CMD_SET_MODE_DEPTH_MODE_DECAL               (3ull << 10ull)
#define     HFX_RDP_CMD_SET_MODE_CVG_DEST_CLAMP                 (0ull << 8ull)
#define     HFX_RDP_CMD_SET_MODE_CVG_DEST_WARP                  (1ull << 8ull)
#define     HFX_RDP_CMD_SET_MODE_CVG_DEST_FULL                  (2ull << 8ull)
#define     HFX_RDP_CMD_SET_MODE_CVG_DEST_SAVE                  (3ull << 8ull)
#define     HFX_RDP_CMD_SET_MODE_IMAGE_READ                     (1ull << 6ull)
#define     HFX_RDP_CMD_SET_MODE_Z_UPDATE_ENABLE                (1ull << 5ull)
#define     HFX_RDP_CMD_SET_MODE_Z_COMPARE_ENABLE               (1ull << 4ull)
/* NOTE: The following values for Z_SOURCE_SEL may be flipped */
#define     HFX_RDP_CMD_SET_MODE_Z_SOURCE_SEL_PIXEL             (0ull << 2ull)
#define     HFX_RDP_CMD_SET_MODE_Z_SOURCE_SEL_PRIM              (1ull << 2ull)
#define HFX_RDP_CMD_TRI_NON_SHADE   0x08ull
#define     HFX_RDP_CMD_TRI_LEFT_MAJOR_FLAG_SHIFT   55ull
#define     HFX_RDP_CMD_TRI_LEVEL_MASK              0x7ull
#define     HFX_RDP_CMD_TRI_LEVEL_SHIFT             51ull
#define     HFX_RDP_CMD_TRI_TILE_MASK               0x7ull
#define     HFX_RPD_CMD_TRI_TILE_SHIFT              48ull
#define     HFX_RDP_CMD_TRI_YL_MASK                 0x7fffull
#define     HFX_RDP_CMD_TRI_YL_SHIFT                32ull
#define     HFX_RDP_CMD_TRI_YM_MASK                 0x7fffull
#define     HFX_RDP_CMD_TRI_YM_SHIFT                16ull
#define     HFX_RDP_CMD_TRI_YH_MASK                 0x7fffull
#define     HFX_RDP_CMD_TRI_YH_SHIFT                0ull
#define HFX_RDP_CMD_TRI_SHADE                       0x0cull
#define HFX_RDP_CMD_TRI_SHADE_DEPTH                 0x0dull
#define HFX_RDP_CMD_SET_BLEND_COLOR                 0x39ull
#define HFX_RDP_CMD_SET_COMBINE_MODE                0x3cull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_A_0_SHIFT      52ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_C_0_SHIFT      47ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_A_0_SHIFT    44ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_C_0_SHIFT    41ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_A_1_SHIFT      37ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_C_1_SHIFT      32ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_B_0_SHIFT      28ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_B_1_SHIFT      24ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_A_1_SHIFT    21ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_C_1_SHIFT    18ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_0_SHIFT      15ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_B_0_SHIFT    12ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_0_SHIFT    9ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_1_SHIFT      6ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_B_1_SHIFT    3ull
#define     HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_1_SHIFT    0ull
#define HFX_RDP_CMD_SYNC_PIPE                       0x27ull


#define HFX_RDP_CMD_SET_BLEND_MODE(type, value) (((value)&HFX_RDP_CMD_SET_MODE_BLEND_MODE_MASK)<<HFX_RDP_CMD_SET_MODE_BLEND_MODE_##type##_SHIFT)


#define HFX_RDP_MAKE_CMD(cmd) ((HFX_RDP_CMD_RESERVE|(cmd)) << HFX_RDP_CMD_SHIFT)
#define HFX_RDP_PKT_SET_COLOR_IMAGE(format, size, width, addr) (uint64_t)(HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_COLOR_IMAGE) | ((uint64_t)(format) << 53) | ((uint64_t)(size) << 51) | (((uint64_t)(width-1)&0x3FF)<<32)  | (uintptr_t)(addr))
#define HFX_RDP_PKT_SET_Z_IMAGE(z_image) (uint64_t)(HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_Z_IMAGE) | (uintptr_t)(z_image))
#define HFX_RDP_PKT_SYNC_PIPE (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SYNC_PIPE))
#define HFX_RDP_PKT_SET_FILL_COLOR(color) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_FILL_COLOR) | (color))
#define HFX_RDP_PKT_FILL_RECT(xl, yl, xh, yh) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_FILL_RECT) | \
                                              (((xl)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_XL_SHIFT) | \
                                              (((yl)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_YL_SHIFT) | \
                                              (((xh)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_XH_SHIFT) | \
                                              (((yh)&HFX_RDP_CMD_FILL_RECT_ARG_MASK) << HFX_RDP_CMD_FILL_RECT_YH_SHIFT))
#define HFX_RDP_PKT_SET_BLEND_COLOR(color) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_BLEND_COLOR)|((uint64_t)(color)))
#define HFX_RDP_PKT_SET_COMBINE_MODE(modes) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_COMBINE_MODE) | ((uint64_t)(modes)))
#define HFX_RDP_PKT_SET_MODE(modes) (HFX_RDP_MAKE_CMD(HFX_RDP_CMD_SET_MODE) | HFX_RPD_CMD_SET_MODE_RESERVED | ((uint64_t)(modes)))
#define HFX_RDP_PKT_TRI_EDGE_COEFF_0(cmd, left_major, level, tile, yl, ym, yh) \
            (HFX_RDP_MAKE_CMD(cmd) | \
            ((uint64_t)(left_major) << HFX_RDP_CMD_TRI_LEFT_MAJOR_FLAG_SHIFT) | \
            ((uint64_t)((level)&HFX_RDP_CMD_TRI_LEVEL_MASK)<<HFX_RDP_CMD_TRI_LEVEL_SHIFT) | \
            ((uint64_t)((tile)&HFX_RDP_CMD_TRI_TILE_MASK)<<HFX_RPD_CMD_TRI_TILE_SHIFT) | \
            ((uint64_t)((yl)&HFX_RDP_CMD_TRI_YL_MASK)<<HFX_RDP_CMD_TRI_YL_SHIFT) | \
            ((uint64_t)((ym)&HFX_RDP_CMD_TRI_YM_MASK)<<HFX_RDP_CMD_TRI_YM_SHIFT) | \
            ((uint64_t)((yh)&HFX_RDP_CMD_TRI_YH_MASK)<<HFX_RDP_CMD_TRI_YH_SHIFT))

/* Before calling any of the triangle draw APIs ensure that a variable called buffer_index exists and is set to the offset you want to begin */
/* writing the triangle draw commands to */
#define HFX_RDP_PKT_TRI_NON_SHADE(buffer, mode, left_major, level, tile, yl, ym, yh, xl, dxLdy, xh, dxHdy, xm, dxMdy) \
            (buffer)[buffer_index++] = HFX_RDP_PKT_TRI_EDGE_COEFF_0(mode, left_major, level, tile, yl, ym, yh); \
            (buffer)[buffer_index++] = ((uint64_t)(xl) << 32ull) | ((uint64_t)(dxLdy)); \
            (buffer)[buffer_index++] = ((uint64_t)(xh) << 32ull) | ((uint64_t)(dxHdy)); \
            (buffer)[buffer_index++] = ((uint64_t)(xm) << 32ull) | ((uint64_t)(dxMdy));
#define HFX_RDP_PKT_TRI_SHADE(buffer, r, g, b, a, drdx, dgdx, dbdx, dadx, drdy, dgdy, dbdy, dady, drde, dgde, dbde, dade) \
            (buffer)[buffer_index++] = ((((uint64_t)(r))&0xFFFF0000) << 32) | ((((uint64_t)(g))&0xFFFF0000) << 16) | ((((uint64_t)(b))&0xFFFF0000)) | ((((uint64_t)(a))&0xFFFF0000) >> 16); \
            (buffer)[buffer_index++] = ((((uint64_t)(drdx))&0xFFFF0000) << 32) | ((((uint64_t)(dgdx))&0xFFFF0000) << 16) | ((((uint64_t)(dbdx))&0xFFFF0000)) | ((((uint64_t)(dadx))&0xFFFF0000) >> 16); \
            (buffer)[buffer_index++] = ((((uint64_t)(r))&0x0000FFFF) << 48) | ((((uint64_t)(g))&0x0000FFFF) << 32) | ((((uint64_t)(b))&0x0000FFFF) << 16) | ((((uint64_t)(a))&0x0000FFFF)); \
            (buffer)[buffer_index++] = ((((uint64_t)(drdx))&0x0000FFFF) << 48) | ((((uint64_t)(dgdx))&0x0000FFFF) << 32) | ((((uint64_t)(dbdx))&0x0000FFFF) << 16) | ((((uint64_t)(dadx))&0x0000FFFF)); \
            (buffer)[buffer_index++] = ((((uint64_t)(drde))&0xFFFF0000) << 32) | ((((uint64_t)(dgde))&0xFFFF0000) << 16) | ((((uint64_t)(dbde))&0xFFFF0000)) | ((((uint64_t)(dade))&0xFFFF0000) >> 16); \
            (buffer)[buffer_index++] = ((((uint64_t)(drdy))&0xFFFF0000) << 32) | ((((uint64_t)(dgdy))&0xFFFF0000) << 16) | ((((uint64_t)(dbdy))&0xFFFF0000)) | ((((uint64_t)(dady))&0xFFFF0000) >> 16); \
            (buffer)[buffer_index++] = ((((uint64_t)(drde))&0x0000FFFF) << 48) | ((((uint64_t)(dgde))&0x0000FFFF) << 32) | ((((uint64_t)(dbde))&0x0000FFFF) << 16) | ((((uint64_t)(dade))&0x0000FFFF)); \
            (buffer)[buffer_index++] = ((((uint64_t)(drdy))&0x0000FFFF) << 48) | ((((uint64_t)(dgdy))&0x0000FFFF) << 32) | ((((uint64_t)(dbdy))&0x0000FFFF) << 16) | ((((uint64_t)(dady))&0x0000FFFF));
#define HFX_RDP_PKT_TRI_DEPTH(buffer, inv_z, dzdx, dzdy, dzde) \
            (buffer)[buffer_index++] = ((uint64_t)(inv_z) << 32) | ((uint64_t)(dzdx)); \
            (buffer)[buffer_index++] = ((uint64_t)(dzde) << 32) | ((uint64_t)(dzdy));

#endif