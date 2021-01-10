#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>

#define RGBA8_TO_RGBA5551(r,g,b,a) ((((r)&0xf8)<<8) | (((g)&0xf8)<<4) | (((b)&0xf8)>>2) | ((a)&0x01))
#define RGBA8_TO_UINT(r,g,b,a)  ((((uint64_t)(r)&0xff)<<24) | (((uint64_t)(g)&0xff)<<16) | (((uint64_t)(b)&0xff)<<8) | ((uint64_t)(a)&0xff))
#define RGBA8_TO_PACKED(r,g,b,a) ((RGBA8_TO_RGBA5551(r,g,b,a) << 16) | (RGBA8_TO_RGBA5551(r,g,b,a)))

extern uint16_t hfx_depth_buffer[];

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

void hfx_tex_coord_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data)
{
    state->tex_coord_pointer = (float*)data;
    state->tex_coord_size = size;
}

void hfx_set_scissor(hfx_state *state, uint32_t xh, uint32_t yh, uint32_t xl, uint32_t yl)
{
    uint64_t cmds[1];

    cmds[0] = HFX_RDP_PKT_SET_SCISSOR(xh, yh, 0, 0, xl, yl);
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
}

void hfx_set_mode(hfx_state *state)
{
    bool is_two_cycle = false; // TODO figure out when we need two cycle
    uint64_t mode = 0;
    uint64_t combine_mode = 0;
    uint64_t cmds[2];
    uint32_t combine_mode_type = HFX_RDP_CMD_SET_COMBINE_MODE_SHADE;

    /* Only update the mode if we need to */
    if(state->caps.dirty)
    {
        if(state->caps.depth_test)
        {
            mode |= HFX_RDP_CMD_SET_MODE_Z_UPDATE_ENABLE | 
                    HFX_RDP_CMD_SET_MODE_Z_COMPARE_ENABLE |
                    HFX_RDP_CMD_SET_MODE_Z_SOURCE_SEL_PIXEL;
        }

        if(is_two_cycle)
        {
            mode |= HFX_RDP_CMD_SET_MODE_TWO_CYCLE;
        }
        else
        {
            mode |= HFX_RDP_CMD_SET_MODE_ONE_CYCLE;
        }

        // TODO need to add more specialized logic for handling different combine modes
        if(state->caps.texture_2d)
        {
            combine_mode_type = HFX_RDP_CMD_SET_COMBINE_MODE_TEXEL0;
            mode |= HFX_RDP_CMD_SET_MODE_BI_LERP_0 |
                    HFX_RDP_CMD_SET_MODE_DETAIL_TEX_EN;
        }

        // TODO set the rest of this state based on the graphics state
        mode |= HFX_RDP_CMD_SET_BLEND_MODE(1B_0, 0) |
                HFX_RDP_CMD_SET_BLEND_MODE(1A_0, 0) |
                HFX_RDP_CMD_SET_BLEND_MODE(2A_0, 0) |
                HFX_RDP_CMD_SET_BLEND_MODE(2B_0, 0) |
                HFX_RDP_CMD_SET_MODE_RGB_NO_DITHER  |
                HFX_RDP_CMD_SET_MODE_ALPHA_NO_DITHER |
                HFX_RDP_CMD_SET_MODE_DEPTH_MODE_OPAQUE |
                HFX_RDP_CMD_SET_MODE_ALPHA_CVG_SELECT |
                HFX_RDP_CMD_SET_MODE_CVG_DEST_FULL;

        combine_mode |= (combine_mode_type<<HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_0_SHIFT) |
                        (combine_mode_type<<HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_0_SHIFT) |
                        (combine_mode_type<<HFX_RDP_CMD_SET_COMBINE_MODE_RGB_D_1_SHIFT) |
                        (combine_mode_type<<HFX_RDP_CMD_SET_COMBINE_MODE_ALPHA_D_1_SHIFT);

        /* Send commands to the RDP */
        cmds[0] = HFX_RDP_PKT_SET_MODE(mode);
        cmds[1] = HFX_RDP_PKT_SET_COMBINE_MODE(combine_mode);

        hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);

        state->caps.dirty = false;
        state->rdp_mode = mode;
    }
}

void hfx_draw_arrays(hfx_state *state, uint32_t type, uint32_t start, uint32_t count)
{
    uint32_t num_tri = count / 3;
    uint32_t start_tri = start / 3;
    float v1[4], v2[4], v3[4], c1[4], c2[4], c3[4], t1[2]={0}, t2[2]={0}, t3[2]={0};
    float tex_width = state->cur_tex->width*32.0f - 1.0f;
    float tex_height= state->cur_tex->height*32.0f - 1.0f;
    uint32_t vs = state->vertex_size;

    hfx_set_mode(state);

    for(int i = start_tri; i < num_tri; i++)
    {
        v1[0] = state->vertex_pointer[i*vs*state->vertex_size+0];
        v1[1] = state->vertex_pointer[i*vs*state->vertex_size+1];
        v1[2] = vs < 3 ? 0.0f : state->vertex_pointer[i*vs*state->vertex_size+2];
        v1[3] = vs < 4 ? 1.0f : state->vertex_pointer[i*vs*state->vertex_size+3];

        v2[0] = state->vertex_pointer[i*vs*state->vertex_size+vs+0];
        v2[1] = state->vertex_pointer[i*vs*state->vertex_size+vs+1];
        v2[2] = vs < 3 ? 0.0f : state->vertex_pointer[i*vs*state->vertex_size+vs+2];
        v2[3] = vs < 4 ? 1.0f : state->vertex_pointer[i*vs*state->vertex_size+vs+3];

        v3[0] = state->vertex_pointer[i*vs*state->vertex_size+vs*2+0];
        v3[1] = state->vertex_pointer[i*vs*state->vertex_size+vs*2+1];
        v3[2] = vs < 3 ? 0.0f : state->vertex_pointer[i*vs*state->vertex_size+vs*2+2];
        v3[3] = vs < 4 ? 1.0f : state->vertex_pointer[i*vs*state->vertex_size+vs*2+3];

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

        if(state->caps.texture_2d)
        {
            t1[0] = state->tex_coord_pointer[i*3*state->tex_coord_size+0]*tex_width;
            t1[1] = state->tex_coord_pointer[i*3*state->tex_coord_size+1]*tex_height;

            t2[0] = state->tex_coord_pointer[i*3*state->tex_coord_size+2]*tex_width;
            t2[1] = state->tex_coord_pointer[i*3*state->tex_coord_size+3]*tex_height;

            t3[0] = state->tex_coord_pointer[i*3*state->tex_coord_size+4]*tex_width;
            t3[1] = state->tex_coord_pointer[i*3*state->tex_coord_size+5]*tex_height;
        }

        hfx_draw_tri_f(state, v1, v2, v3, c1, c2, c3, t1, t2, t3);
    }
}

void hfx_clear(hfx_state *state, uint32_t bits)
{
    uint32_t index = 0;
    uint64_t cmds[1+4+2+1];

    cmds[index++] = HFX_RDP_PKT_SET_MODE(HFX_RDP_CMD_SET_MODE_ATOMIC_PRIM |
                                   HFX_RDP_CMD_SET_MODE_FILL |
                                   HFX_RDP_CMD_SET_MODE_RGB_NO_DITHER |
                                   HFX_RDP_CMD_SET_MODE_ALPHA_NO_DITHER) ;
    if(bits & HFX_DEPTH_BUFFER_BIT)
    {
        uint32_t packed_color = (HFX_PACK_Z_VALUE(HFX_MAX_DEPTH_VALUE,0)<<16 | HFX_PACK_Z_VALUE(HFX_MAX_DEPTH_VALUE,0));
        cmds[index++] = HFX_RDP_PKT_SET_COLOR_IMAGE(HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_RGBA, HFX_RDP_CMD_SET_COLOR_IMAGE_SIZE_16B, state->display_dim.width, 0x1ffffff&(uintptr_t)hfx_depth_buffer);
        cmds[index++] = HFX_RDP_PKT_SET_FILL_COLOR(packed_color);
        cmds[index++] = HFX_RDP_PKT_FILL_RECT(state->display_dim.width << 2, state->display_dim.height << 2, 0, 0);
        cmds[index++] = HFX_RDP_PKT_SET_COLOR_IMAGE(HFX_RDP_CMD_SET_COLOR_IMAGE_FORMAT_RGBA, HFX_RDP_CMD_SET_COLOR_IMAGE_SIZE_16B, state->display_dim.width, hfx_display_get_pointer(state));
    }

    /* If we are clearing color buffer */
    if(bits & HFX_COLOR_BUFFER_BIT)
    {
        uint32_t packed_color = RGBA8_TO_PACKED(state->clear_color.r, state->clear_color.g, state->clear_color.b, state->clear_color.a);
        cmds[index++] = HFX_RDP_PKT_SET_FILL_COLOR(packed_color);
        cmds[index++] = HFX_RDP_PKT_FILL_RECT(state->display_dim.width << 2, state->display_dim.height << 2, 0, 0);
    }
    cmds[index++] = HFX_RDP_PKT_SET_MODE(state->rdp_mode);

    hfx_cmd_rdp(state, index, cmds);
}

void hfx_draw_tri_f(hfx_state *state, float *v1, float *v2, float *v3, float *vc1, float *vc2, float *vc3, float *vt1, float *vt2, float *vt3)
{
    float v1_t[4], v2_t[4], v3_t[4];

    hfx_matrix_vector_multiply(state, state->model_matrix, v1, v1_t);
    hfx_matrix_vector_multiply(state, state->model_matrix, v2, v2_t);
    hfx_matrix_vector_multiply(state, state->model_matrix, v3, v3_t);

    hfx_render_tri_f(state, v1_t, v2_t, v3_t, vc1, vc2, vc3, vt1, vt2, vt3);
}