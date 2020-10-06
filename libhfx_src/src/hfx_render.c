#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>

void hfx_render_init(hfx_state *state)
{
    uint32_t cmds = 
    {
        /* Enable primitive fill */
        0xEF0000FF,
        0x80000000,
    };

    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint32_t),cmds);
}

void hfx_set_primitive_color(hfx_state *state, uint32_t color)
{
    uint32_t cmds = 
    {
        0xF9000000,
        0x00000000,
    };
    cmds[1] = color;
    
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint32_t), cmds);

}

void hfx_render_tri(hfx_state *state, HFXfixed *v1, HFXfixed *v2, HFXfixed *v3)
{
    uint32_t a1 = (v1[0] & 0xFFFF0000) | ((v1[1] & 0xFFFF0000) >> 16);
    uint32_t a2 = (v1[2] & 0xFFFF0000) | ((v1[3] & 0xFFFF0000) >> 16);
    uint32_t b1 = (v2[0] & 0xFFFF0000) | ((v2[1] & 0xFFFF0000) >> 16);
    uint32_t b2 = (v2[2] & 0xFFFF0000) | ((v2[3] & 0xFFFF0000) >> 16);
    uint32_t c1 = (v3[0] & 0xFFFF0000) | ((v3[1] & 0xFFFF0000) >> 16);
    uint32_t c2 = (v3[2] & 0xFFFF0000) | ((v3[3] & 0xFFFF0000) >> 16);

    uint32_t d1 = ((v1[0] & 0x0000FFFF) << 16) | (v1[1] & 0x0000FFFF);
    uint32_t d2 = ((v1[2] & 0x0000FFFF) << 16) | (v1[3] & 0x0000FFFF);
    uint32_t e1 = ((v2[0] & 0x0000FFFF) << 16) | (v2[1] & 0x0000FFFF);
    uint32_t e2 = ((v2[2] & 0x0000FFFF) << 16) | (v2[3] & 0x0000FFFF);
    uint32_t f1 = ((v3[0] & 0x0000FFFF) << 16) | (v3[1] & 0x0000FFFF);
    uint32_t f2 = ((v3[2] & 0x0000FFFF) << 16) | (v3[3] & 0x0000FFFF);

    hfx_rb_reserve(state, 13);
    hfx_rb_queue(state, HFX_CMD_TRI);
    hfx_rb_queue(state, a1);
    hfx_rb_queue(state, a2);
    hfx_rb_queue(state, b1);
    hfx_rb_queue(state, b2);
    hfx_rb_queue(state, c1);
    hfx_rb_queue(state, c2);
    hfx_rb_queue(state, d1);
    hfx_rb_queue(state, d2);
    hfx_rb_queue(state, e1);
    hfx_rb_queue(state, e2);
    hfx_rb_queue(state, f1);
    hfx_rb_queue(state, f2);
}