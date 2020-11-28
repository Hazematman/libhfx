#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>
#include <stdio.h>
#include <libdragon.h>

#define get_disp_buffer(x) __safe_buffer[(x)-1]

uint16_t hfx_depth_buffer[320*240] __attribute__((aligned(8)));

extern void *__safe_buffer[];

void *hfx_display_get_pointer(hfx_state *state)
{
    return get_disp_buffer(state->display);
}

void hfx_cmd_register_display(hfx_state *state)
{
    uint64_t buffer_cmds[2];
    /* Get display handle */
    while(!(state->display = display_lock()));
    
    hfx_rb_reserve(state, 2);
    hfx_rb_queue(state, HFX_CMD_SET_DISPLAY);
    hfx_rb_queue(state, (uint32_t)get_disp_buffer(state->display));

    buffer_cmds[0] = HFX_RDP_PKT_SET_Z_IMAGE(hfx_depth_buffer);
    buffer_cmds[1] = HFX_RDP_PKT_SYNC_PIPE;
    hfx_cmd_rdp(state, sizeof(buffer_cmds)/sizeof(uint64_t), buffer_cmds);
}


void hfx_swap_buffers(hfx_state *state)
{
    display_show(state->display);
    hfx_cmd_register_display(state); 
}