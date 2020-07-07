#ifndef HFX_RB_H
#define HFX_RB_H
#include <hfx_regs.h>

void hfx_rb_calc_size_mask(hfx_state *state);
void hfx_rb_reserve(hfx_state *state, uint32_t num_cmds);
void hfx_rb_queue(hfx_state *state, uint32_t cmd);
void hfx_rb_submit(hfx_state *state);

#endif