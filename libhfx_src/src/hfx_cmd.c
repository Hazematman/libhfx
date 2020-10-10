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

void hfx_cmd_rdp(hfx_state *state, uint32_t num_cmds, uint64_t *cmds)
{
    uint32_t cmd = ((num_cmds << 8) | HFX_CMD_SEND_RDP);
    hfx_rb_reserve(state, 1+num_cmds);
    hfx_rb_queue(state, cmd);
    for(int i=0; i < num_cmds; i++)
    {
        hfx_rb_queue(state, (uint32_t)(cmds[i] >> 32));
        hfx_rb_queue(state, (uint32_t)(cmds[i]));
    }
}