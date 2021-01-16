#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <hfx.h>
#include <hfx_rb.h>
#include <hfx_cmds.h>
#include <hfx_int.h>
#include <hfx_types.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static int done = 1;
static hfx_state *state;

static char pbuf[256];
float v1[] =
{
    -0.8f, -0.8f, -0.1f,
    0.0f, -0.8f, -0.8f,
    0.0f, 0.8f, -0.8f,

    -0.8f, -0.8f, -0.1f,
    0.0f, 0.8f, -0.8f,
    -0.8f, 0.8f, -0.1f,

};

uint8_t vc1[] =
{
    255.0f, 0.0f, 0.0f, 255.0f,
    255.0f, 0.0f, 0.0f, 255.0f,
    255.0f, 0.0f, 0.0f, 255.0f,

    255.0f, 0.0f, 0.0f, 255.0f,
    255.0f, 0.0f, 0.0f, 255.0f,
    255.0f, 0.0f, 0.0f, 255.0f,
};

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );

    state = hfx_init();

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 0, v1);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 0, vc1);
    hfx_clear_color_f(state, 0.0f, 0.0f, 0.0f, 1.0f);

    float angle = 10.0f;

    while(1)
    {
        angle += 1.0f;
        // Queue the next frame up
        hfx_clear(state, HFX_COLOR_BUFFER_BIT|HFX_DEPTH_BUFFER_BIT);
        hfx_load_identity(state);
        hfx_persp_f(state, 120.0f, 320.0f/240.0f, 0.1f, 100.0f);

        hfx_rotate_f(state, angle, 0.0f, 1.0f, 0.0f);

        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 6);

        hfx_swap_buffers(state);
    }
}