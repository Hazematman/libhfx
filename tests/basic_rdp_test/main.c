#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <hfx.h>
#include <hfx_rb.h>
#include <hfx_cmds.h>
#include <hfx_int.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static uint32_t array[4] __attribute__((aligned(8))) = {1,2,3,4};
static uint32_t array2[4] __attribute__((aligned(8))) = {5,6,7,8};
static uint32_t buffer[1024] __attribute__((aligned(8)));

static int done = 1;

static void hfx_int()
{
    done = 0;
}

static uint32_t cmds[] =
{
    0xED000000,
    0x005003C0,
    0xEFB000FF,
    0x00004000,
    0xF7000000,
    0xFF00FF00,
    0xF6190190,
    0x00000000,
    0xE9000000,
    0x00000000,
};

int main(void)
{
     /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );

    register_DP_handler(hfx_int);
    set_DP_interrupt(1);

    hfx_state *state = hfx_init();
    hfx_register_rsp_int(state, hfx_int);
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint32_t), cmds);
    hfx_rb_submit(state);


    while(1)
    {
        if(done == 0)
        {
            done = 1;

            hfx_swap_buffers(state);
        }
    }
}