#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <libdragon.h>

#define get_disp_buffer(x) __safe_buffer[(x)-1]

extern void *__safe_buffer[];

void hfx_cmd_register_display(hfx_state *state)
{
    /* Get display handle */
    while(!(state->display = display_lock()));

    hfx_rb_reserve(state, 2);
    hfx_rb_queue(state, HFX_CMD_SET_DISPLAY);
    hfx_rb_queue(state, (uint32_t)get_disp_buffer(state->display));
}


void hfx_swap_buffers(hfx_state *state)
{
    display_show(state->display);    
}