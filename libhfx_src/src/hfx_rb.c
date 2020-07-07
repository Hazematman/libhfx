#include <hfx.h>
#include <hfx_types.h>
#include <hfx_rb.h>
#include <hfx_int.h>
#include <libdragon.h>
#include <stdint.h>

void hfx_rb_calc_size_mask(hfx_state *state)
{
    // TODO replace this would properly calculated mask
    state->rb_size_mask = 0x3FF;
}

void hfx_rb_reserve(hfx_state *state, uint32_t num_cmds)
{
    uint32_t num_bytes = num_cmds*4;
    uint32_t rb_start = 0;
    uint32_t rb_size = 0;
    uint32_t num_bytes_in_buffer = 0;

    /* If command buffer is not empty */
    if(state->rb_end != hfx_read_reg(HFX_VADDR_REG_RB_START))
    {
        /* While there is not enough space in the command buffer */
        do {
            rb_start = hfx_read_reg(HFX_VADDR_REG_RB_START);
            num_bytes_in_buffer = (state->rb_end - rb_start + 4) % state->rb_size_mask;
            rb_size = state->rb_size - num_bytes_in_buffer;
        } while(rb_size < num_bytes);
    }
}


void hfx_rb_queue(hfx_state *state, uint32_t cmd)
{
    state->rb[state->rb_end/4] = cmd;
    state->rb_end = (state->rb_end + 4) & state->rb_size_mask;
}

void hfx_rb_submit(hfx_state *state)
{
    // TODO likely need to decrement rb_end by 4 here
    /* Flush cache to make sure all commands are written to memory for DMA */
    data_cache_hit_writeback_invalidate(state->rb, state->rb_size);
    hfx_write_reg(HFX_VADDR_REG_RB_END, state->rb_end);
}