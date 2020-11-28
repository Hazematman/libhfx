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

/* This command sets the clip area */
/* Should not be required, internal library code should */
/* Handle this on init */
static uint64_t cmds1[] =
{
    0xED000000005003C0ULL,
};

/* This triggers a full sync to happen */
/* eventually this should all be moved into the swap buffers command */
static uint64_t cmds[] =
{
    0xE900000000000000ULL,
};

static void hfx_int()
{
    done = 0;
}

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

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );

    register_DP_handler(hfx_int);
    set_DP_interrupt(1);

    state = hfx_init();
    hfx_register_rsp_int(state, hfx_int); 

    hfx_cmd_rdp(state, sizeof(cmds1)/sizeof(uint64_t), cmds1);

    hfx_clear_color_f(state, 0.3f, 0.1f, 0.9f, 1.0f);

#if 0
    hfxEnable(state, HFX_DEPTH_TEST);
    hfxEnable(state, HFX_VERTEX_ARRAY);
    hfxEnable(state, HFX_COLOR_ARRAY);
#endif

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 0, cube_verts);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 0, cube_colors);

    hfx_clear(state, HFX_COLOR_BUFFER_BIT | HFX_DEPTH_BUFFER_BIT);
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_rb_submit(state);


    float angle = 0;

    while(1)
    {
        if(done == 0)
        {
            if(angle == 360)
                angle = 0;
            else
                angle += 1.0f;
            done = 1;

            hfx_swap_buffers(state);

            // Queue the next frame up
            hfx_clear(state, HFX_COLOR_BUFFER_BIT | HFX_DEPTH_BUFFER_BIT);
            hfx_load_identity(state);
            hfx_scale_f(state, 320.0f/2.0f, 240.0f/2.0f, 1.0f);
            hfx_translate_f(state, 1.0f, 1.0f, 0.0f);
            hfx_translate_f(state, 0.0f, 0.0f, 0.5f);
            hfx_rotate_f(state, angle, 0.58f, 0.58f, 0.58f);
            hfx_scale_f(state, 0.2f, 0.2f, 0.2f);
            hfx_draw_arrays(state, HFX_TRIANGLES, 0, 36);
            hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
            hfx_rb_submit(state);
        }
    }
}