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

/* With current implementation RDP buffer is 256 bytes long */
/* which means only 32 64-bit commands can fit in the buffer */
/* but during init the hfx library sends a single command to set the */
/* Framebuffer address. So we will send 30 commands to start before sending */
/* the commands that we actually want to do work */
#define WRAP_LIMIT 30

#define BUFFER_SIZE_BYTES 256

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static int done = 1;
static hfx_state *state;

static uint64_t cmds1[] =
{
    0xED000000005003C0ULL,
};

static uint64_t cmds[] =
{
    0xE900000000000000ULL,
};

static void hfx_int()
{
    done = 0;
}

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

    // TODO this rdp command is only here to set the default clip
    // Should really be part of the hfx library
    hfx_cmd_rdp(state, sizeof(cmds1)/sizeof(uint64_t), cmds1);
    hfx_clear_color_f(state, 1.0f, 0.0f, 0.0f, 1.0f);
    hfx_clear(state, HFX_COLOR_BUFFER_BIT);
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_rb_submit(state);


    while(1)
    {
        if(done == 0)
        {
            done = 1;

            graphics_draw_text(state->display, 0, 100, "Done");

            hfx_swap_buffers(state);
        }
    }
}