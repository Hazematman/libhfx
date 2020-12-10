#ifndef HFX_H
#define HFX_H
#include <hfx_cmds.h>
#include <stdint.h>

#define HFX_COLOR_BUFFER_BIT	0x00004000
#define HFX_DEPTH_BUFFER_BIT	0x00000100

#define HFX_BYTE			    0x1400
#define HFX_UNSIGNED_BYTE		0x1401
#define HFX_SHORT				0x1402
#define HFX_UNSIGNED_SHORT		0x1403
#define HFX_INT					0x1404
#define HFX_UNSIGNED_INT		0x1405
#define HFX_FLOAT				0x1406

#define HFX_TRIANGLES			0x0004

#define HFX_VERTEX_ARRAY        0x8074
#define HFX_COLOR_ARRAY         0x8076

#define HFX_DEPTH_TEST			0x0B71

typedef struct hfx_state hfx_state;
typedef int32_t HFXfixed;

hfx_state *hfx_init();
void hfx_register_rsp_int(hfx_state *state, void *func_ptr);
void hfx_restart_rsp(hfx_state *state);
void hfx_wait_for_idle(hfx_state *state);

void hfx_swap_buffers(hfx_state *state);

HFXfixed hfx_addx(HFXfixed a, HFXfixed b);
HFXfixed hfx_subx(HFXfixed a, HFXfixed b);
HFXfixed hfx_mulx(HFXfixed a, HFXfixed b);
HFXfixed hfx_divx(HFXfixed a, HFXfixed b);


void hfxEnable(hfx_state *state, uint32_t cap);
void hfx_enable(hfx_state *state, uint32_t cap);
void hfxClearColor(hfx_state *state, float r, float g, float b, float a);
void hfxClear(hfx_state *state, uint32_t bits);
void hfxVertexPointer(hfx_state *state, int32_t size, uint32_t type, uint32_t stride, void *pointer);
void hfxColorPointer(hfx_state *state, int32_t size, uint32_t type, uint32_t stride, void *pointer);
void hfxLoadIdentity(hfx_state *state);
void hfxTranslatef(hfx_state *state, float x, float y, float z);
void hfxRotatef(hfx_state *state, float angle, float x, float y, float z);
void hfxScalef(hfx_state *state, float x, float y, float z);
void hfxDrawArrays(hfx_state *state, uint32_t mode, uint32_t first, uint32_t count);

void hfx_draw_tri_f(hfx_state *state, float *v1, float *v2, float *v3, float *vc1, float *vc2, float *vc3);
void hfx_color_f(hfx_state *state, float r, float g, float b, float a);
void hfx_clear_color_f(hfx_state *state, float r, float g, float b, float a);
void hfx_clear(hfx_state *state, uint32_t bits);
void hfx_load_identity(hfx_state *state);
void hfx_translate_f(hfx_state *state, float x, float y, float z);
void hfx_rotate_f(hfx_state *state, float angle, float x, float y, float z);
void hfx_scale_f(hfx_state *state, float sx, float sy, float sz);
void hfx_ortho_f(hfx_state *state, float left, float right, float top, float bottom, float near, float far);

void hfx_vertex_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data);
void hfx_color_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data);
void hfx_draw_arrays(hfx_state *state, uint32_t type, uint32_t start, uint32_t count);


#endif
