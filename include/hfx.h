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

#define HFX_UNSIGNED_SHORT_5_5_5_1  0x8034

#define HFX_TEXTURE_2D          0x0de1

#define HFX_RGBA                0x1908

#define HFX_TRIANGLES			0x0004

#define HFX_VERTEX_ARRAY        0x8074
#define HFX_COLOR_ARRAY         0x8076

#define HFX_DEPTH_TEST			0x0B71

typedef struct hfx_state hfx_state;
typedef int32_t HFXfixed;

hfx_state *hfx_init();
void libhfx_init(void);
void hfx_register_rsp_int(hfx_state *state, void (*func_ptr)());
void hfx_restart_rsp(hfx_state *state);
void hfx_wait_for_idle(hfx_state *state);

void hfx_swap_buffers(hfx_state *state);

HFXfixed hfx_addx(HFXfixed a, HFXfixed b);
HFXfixed hfx_subx(HFXfixed a, HFXfixed b);
HFXfixed hfx_mulx(HFXfixed a, HFXfixed b);
HFXfixed hfx_divx(HFXfixed a, HFXfixed b);

void hfx_enable(hfx_state *state, uint32_t cap);

void hfx_set_scissor(hfx_state *state, uint32_t xh, uint32_t yh, uint32_t xl, uint32_t yl);
void hfx_draw_tri_f(hfx_state *state, float *v1, float *v2, float *v3, float *vc1, float *vc2, float *vc3, float *vt1, float *vt2, float *vt3);
void hfx_color_f(hfx_state *state, float r, float g, float b, float a);
void hfx_clear_color_f(hfx_state *state, float r, float g, float b, float a);
void hfx_clear(hfx_state *state, uint32_t bits);
void hfx_load_identity(hfx_state *state);
void hfx_translate_f(hfx_state *state, float x, float y, float z);
void hfx_rotate_f(hfx_state *state, float angle, float x, float y, float z);
void hfx_scale_f(hfx_state *state, float sx, float sy, float sz);
void hfx_ortho_f(hfx_state *state, float left, float right, float top, float bottom, float near, float far);
void hfx_persp_f(hfx_state *state, float fovy, float aspect, float znear, float zfar);
void hfx_mult_matrix_f(hfx_state *state, float *mat);

void hfx_vertex_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data);
void hfx_color_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data);
void hfx_tex_coord_pointer(hfx_state *state, uint32_t size, uint32_t type, uint32_t stride, void *data);
void hfx_draw_arrays(hfx_state *state, uint32_t type, uint32_t start, uint32_t count);

void hfx_gen_textures(hfx_state *state, uint32_t n, uint32_t *textures);
void hfx_tex_image_2d(hfx_state *state, uint32_t target, int32_t level, int32_t internalformat, uint32_t width, uint32_t height, int32_t border, uint32_t format, uint32_t type, const void *data);

#endif
