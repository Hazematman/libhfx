#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>

#define RGBA8_TO_RGBA5551(r,g,b,a) ((((r)&0xf8)<<8) | (((g)&0xf8)<<4) | (((b)&0xf8)>>2) | ((a)&0x01))
#define RGBA8_TO_UINT(r,g,b,a)  ((((uint64_t)(r)&0xff)<<24) | (((uint64_t)(g)&0xff)<<16) | (((uint64_t)(b)&0xff)<<8) | ((uint64_t)(a)&0xff))
#define RGBA8_TO_PACKED(r,g,b,a) ((RGBA8_TO_RGBA5551(r,g,b,a) << 16) | (RGBA8_TO_RGBA5551(r,g,b,a)))


void hfx_clear_color_f(hfx_state *state, float r, float g, float b, float a)
{
    state->clear_color.r = r*255;
    state->clear_color.g = g*255;
    state->clear_color.b = b*255;
    state->clear_color.a = a*255;
}

void hfx_color_f(hfx_state *state, float r, float g, float b, float a)
{
    state->vertex_color.r = r*255;
    state->vertex_color.g = g*255;
    state->vertex_color.b = b*255;
    state->vertex_color.a = a*255;
}

void hfx_clear(hfx_state *state, uint32_t bits)
{
    /* If we are clearing color buffer */
    if(bits & HFX_COLOR_BUFFER_BIT)
    {
        uint32_t packed_color = RGBA8_TO_PACKED(state->clear_color.r, state->clear_color.g, state->clear_color.b, state->clear_color.a);
        uint64_t cmds[4];
        cmds[0] = HFX_RDP_PKT_SET_MODE(HFX_RDP_CMD_SET_MODE_ATOMIC_PRIM|HFX_RDP_CMD_SET_MODE_FILL|HFX_RDP_CMD_SET_MODE_RGB_NO_DITHER|HFX_RDP_CMD_SET_MODE_ALPHA_NO_DITHER|HFX_RDP_CMD_SET_MODE_FORCE_BLEND);
        cmds[1] = HFX_RDP_PKT_SET_FILL_COLOR(packed_color);
        cmds[2] = HFX_RDP_PKT_FILL_RECT(state->display_dim.width << 2, state->display_dim.height << 2, 0, 0);
        cmds[3] = HFX_RDP_PKT_SYNC_PIPE;

        hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    }   
}

void hfx_draw_tri_f(hfx_state *state, float *v1, float *v2, float *v3)
{
    uint64_t cmds[3];
    float v1_t[4], v2_t[4], v3_t[4];

    hfx_matrix_vector_multiply(state, state->model_matrix, v1, v1_t);
    hfx_matrix_vector_multiply(state, state->model_matrix, v2, v2_t);
    hfx_matrix_vector_multiply(state, state->model_matrix, v3, v3_t);


    cmds[0] = HFX_RDP_PKT_SET_MODE(HFX_RDP_CMD_SET_BLEND_MODE(1A_0, 0) |
                                   HFX_RDP_CMD_SET_BLEND_MODE(1B_0, 0) |
                                   HFX_RDP_CMD_SET_BLEND_MODE(2A_0, 0) |
                                   HFX_RDP_CMD_SET_BLEND_MODE(2B_0, 0) |
                                   HFX_RDP_CMD_SET_MODE_RGB_NO_DITHER  |
                                   HFX_RDP_CMD_SET_MODE_ALPHA_NO_DITHER);
    cmds[1] = HFX_RDP_PKT_SET_BLEND_COLOR(RGBA8_TO_UINT(state->vertex_color.r, 
                                                        state->vertex_color.g,
                                                        state->vertex_color.b,
                                                        state->vertex_color.a));
    cmds[2] = HFX_RDP_PKT_SET_COMBINE_MODE( (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_0_SHIFT) |
                                            (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_0_SHIFT) |
                                            (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_1_SHIFT) |
                                            (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_1_SHIFT));

    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_render_tri_f(state, v1_t, v2_t, v3_t);
}