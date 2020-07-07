#ifndef HFX_H
#define HFX_H
#include <hfx_cmds.h>

typedef struct hfx_state hfx_state;

hfx_state *hfx_init();
void hfx_register_rsp_int(hfx_state *state, void *func_ptr);
void hfx_restart_rsp(hfx_state *state);

#endif
