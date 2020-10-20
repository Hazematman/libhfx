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
void hfx_cmd_register_display(hfx_state *state);
void hfx_cmd_rdp(hfx_state *state, uint32_t num_cmds, uint64_t *cmds);

void hfx_matrix_vector_multiply(hfx_state *state, float *mat, float *vec, float *result);
void hfx_render_tri_f(hfx_state *state, float *v1, float *v2, float *v3, float *vc1, float *vc2, float *vc3);


#endif