#ifndef HFX_H
#define HFX_H
#include <hfx_cmds.h>
#include <stdint.h>

typedef struct hfx_state hfx_state;
typedef int32_t HFXfixed;

hfx_state *hfx_init();
void hfx_register_rsp_int(hfx_state *state, void *func_ptr);
void hfx_restart_rsp(hfx_state *state);

void hfx_swap_buffers(hfx_state *state);

HFXfixed hfx_addx(HFXfixed a, HFXfixed b);
HFXfixed hfx_subx(HFXfixed a, HFXfixed b);
HFXfixed hfx_mulx(HFXfixed a, HFXfixed b);
HFXfixed hfx_divx(HFXfixed a, HFXfixed b);

#endif
