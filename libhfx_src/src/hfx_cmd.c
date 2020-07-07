#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>

void hfx_cmd_nop(hfx_state *state)
{
    hfx_rb_reserve(state, 1);
    hfx_rb_queue(state, HFX_CMD_NOP);
}

void hfx_cmd_int(hfx_state *state)
{
    hfx_rb_reserve(state, 1);
    hfx_rb_queue(state, HFX_CMD_INT);
}