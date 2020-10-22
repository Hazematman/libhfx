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

void hfx_vertex_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data)
{
    state->vertex_pointer = (float*)data;
    state->vertex_size = size;
}

void hfx_color_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data)
{
    state->color_pointer = (uint8_t*)data;
    state->color_size = size;
}

void hfx_draw_arrays(hfx_state *state, uint32_t type, uint32_t start, uint32_t count)
{
    uint32_t num_tri = count / 3;
    float v1[4], v2[4], v3[4], c1[4], c2[4], c3[4];

    for(int i = 0; i < num_tri; i++)
    {
        v1[0] = state->vertex_pointer[i*3*state->vertex_size+0];
        v1[1] = state->vertex_pointer[i*3*state->vertex_size+1];
        v1[2] = state->vertex_pointer[i*3*state->vertex_size+2];
        v1[3] = 1.0f;

        v2[0] = state->vertex_pointer[i*3*state->vertex_size+3];
        v2[1] = state->vertex_pointer[i*3*state->vertex_size+4];
        v2[2] = state->vertex_pointer[i*3*state->vertex_size+5];
        v2[3] = 1.0f;

        v3[0] = state->vertex_pointer[i*3*state->vertex_size+6];
        v3[1] = state->vertex_pointer[i*3*state->vertex_size+7];
        v3[2] = state->vertex_pointer[i*3*state->vertex_size+8];
        v3[3] = 1.0f;

        c1[0] = (float)state->color_pointer[i*3*state->color_size+0];
        c1[1] = (float)state->color_pointer[i*3*state->color_size+1];
        c1[2] = (float)state->color_pointer[i*3*state->color_size+2];
        c1[3] = (float)state->color_pointer[i*3*state->color_size+3];

        c2[0] = (float)state->color_pointer[i*3*state->color_size+4];
        c2[1] = (float)state->color_pointer[i*3*state->color_size+5];
        c2[2] = (float)state->color_pointer[i*3*state->color_size+6];
        c2[3] = (float)state->color_pointer[i*3*state->color_size+7];

        c3[0] = (float)state->color_pointer[i*3*state->color_size+8];
        c3[1] = (float)state->color_pointer[i*3*state->color_size+9];
        c3[2] = (float)state->color_pointer[i*3*state->color_size+10];
        c3[3] = (float)state->color_pointer[i*3*state->color_size+11];

        hfx_draw_tri_f(state, v1, v2, v3, c1, c2, c3);
    }
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

void hfx_draw_tri_f(hfx_state *state, float *v1, float *v2, float *v3, float *vc1, float *vc2, float *vc3)
{
    uint64_t cmds[2];
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
    cmds[1] = HFX_RDP_PKT_SET_COMBINE_MODE( (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_0_SHIFT) |
                                            (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_0_SHIFT) |
                                            (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_1_SHIFT) |
                                            (4ull<<HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_1_SHIFT));

    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_render_tri_f(state, v1_t, v2_t, v3_t, vc1, vc2, vc3);
    
    /* TODO this submit should not be needed */
    hfx_rb_submit(state);
}