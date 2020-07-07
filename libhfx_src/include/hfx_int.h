#ifndef HFX_INT_H
#define HFX_INT_H
#include <stdint.h>

void hfx_write_reg(uint32_t addr, uint32_t data);
uint32_t hfx_read_reg(uint32_t addr);

/* Internal HFX cmds */
void hfx_cmd_nop(hfx_state *state);
void hfx_cmd_int(hfx_state *state);
void hfx_cmd_dma_read_to_rsp(
    hfx_state *state, uint32_t dmem_addr, void *addr, uint32_t size);


#endif