#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>


#define RGBA8_TO_RGBA5551(r,g,b,a) ((((r)&0xf8)<<8) | (((g)&0xf8)<<4) | (((b)&0xf8)>>2) | ((a)&0x01))
#define RGBA8_TO_PACKED(r,g,b,a) ((RGBA8_TO_RGBA5551(r,g,b,a) << 16) | (RGBA8_TO_RGBA5551(r,g,b,a) << 16))


void hfx_clear_color_f(hfx_state *state, float r, float g, float b, float a)
{
    state->clear_color.r = r*255;
    state->clear_color.g = g*255;
    state->clear_color.b = b*255;
    state->clear_color.a = a*255;
}

void hfx_clear(hfx_state *state, uint32_t bits)
{
    /* If we are clearing color buffer */
    if(bits & HFX_COLOR_BUFFER_BIT)
    {
        uint32_t packed_color = RGBA8_TO_PACKED(state->clear_color.r, state->clear_color.g, state->clear_color.b, state->clear_color.a);
        uint64_t cmds[3];
        cmds[1] = HFX_RDP_PKT_SET_FILL_COLOR(packed_color);
        cmds[2] = HFX_RDP_PKT_FILL_RECT(state->display_dim.width << 2, state->display_dim.height << 2, 0, 0);

        hfx_cmd_rdp(state, sizeof(cmds), cmds);
    }   
}