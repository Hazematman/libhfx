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

static hfx_state *state;

static char pbuf[256];

float cube_verts[] = 
{
    // Top
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,

    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    // Front
    -1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,

    // Right
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

    // Left
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,

    // Back
    -1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    // Bottom
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,

};

uint8_t cube_colors[] =
{
    // Top
    255, 0, 0, 255,
    255, 0, 0, 255,
    255, 0, 0, 255,

    255, 0, 0, 255,
    255, 0, 0, 255,
    255, 0, 0, 255,

    // Front
    0, 255, 0, 255,
    0, 255, 0, 255,
    0, 255, 0, 255,

    0, 255, 0, 255,
    0, 255, 0, 255,
    0, 255, 0, 255,

    // Right
    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    // Left
    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    0, 0, 255, 255,
    0, 0, 255, 255,
    0, 0, 255, 255,

    // Back
    255, 100, 0, 255,
    255, 100, 0, 255,
    255, 100, 0, 255,

    255, 100, 0, 255,
    255, 100, 0, 255,
    255, 100, 0, 255,

    // Bottom
    0, 255, 255, 255,
    0, 255, 255, 255,
    0, 255, 255, 255,

    0, 255, 255, 255,
    0, 255, 255, 255,
    0, 255, 255, 255,
};

void exception(exception_t *data)
{
    hfx_fatal_error(state, "exception");
}

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    register_exception_handler(exception);

    state = hfx_init();

    hfx_clear_color_f(state, 0.3f, 0.1f, 0.9f, 1.0f);

    hfx_enable(state, HFX_DEPTH_TEST);
    hfx_enable(state, HFX_VERTEX_ARRAY);
    hfx_enable(state, HFX_COLOR_ARRAY);

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 0, cube_verts);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 0, cube_colors);

    float angle = 0;
    while(1)
    {
        if(angle == 360)
        {
            angle = 0;
        }
        else
        {
            angle += 1.0f;
        }

        //angle = 285.0f;

        // Queue the next frame up
        hfx_clear(state, HFX_COLOR_BUFFER_BIT | HFX_DEPTH_BUFFER_BIT);
        hfx_load_identity(state);
        hfx_rotate_f(state, angle, 0.58f, 0.58f, 0.58f);
        hfx_scale_f(state, 0.5f, 0.5f, 0.5f);
        //hfx_scale_f(state, 1, -1, 1);
        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 36);

        hfx_wait_for_idle(state);

        sprintf(pbuf, "Done %f", angle);
        graphics_draw_text(state->display, 0, 100, pbuf);

        hfx_swap_buffers(state);
    }
}