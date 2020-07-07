#include <hfx.h>
#include <hfx_types.h>
#include <hfx_rb.h>

void hfx_cmd_dma_read_to_rsp(
    hfx_state *state, uint32_t dmem_addr, void *addr, uint32_t size)
{
    hfx_rb_reserve(state, 4);
    hfx_rb_queue(state, HFX_CMD_DMA_READ);
    hfx_rb_queue(state, dmem_addr);
    hfx_rb_queue(state, (uint32_t)addr);
    hfx_rb_queue(state, size-1);
}