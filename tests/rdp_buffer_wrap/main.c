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

static char print_buf[256];

static void hfx_int()
{
    done = 0;
}

static uint64_t null_cmds[WRAP_LIMIT];

static uint64_t cmds[] =
{
    0xED000000005003C0ULL,
    0xEFB000FF00004000ULL,
    0xF7000000FF00FF00ULL,
    0xF619019000000000ULL,
    0xE900000000000000ULL,
};

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );

    register_DP_handler(hfx_int);
    set_DP_interrupt(1);

    for(uint32_t i = 0; i < WRAP_LIMIT; i++)
    {
        null_cmds[i] = 0xED000000005003C0ULL;
    }

    state = hfx_init();
    hfx_register_rsp_int(state, hfx_int);


    hfx_cmd_rdp(state, sizeof(null_cmds)/sizeof(uint64_t), null_cmds);
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_rb_submit(state);


    while(1)
    {
        if(done == 0)
        {
            //done = 1;
            uint32_t rdp_start = hfx_read_reg(0x04100000);
            uint32_t rdp_end = hfx_read_reg(0x04100004);
            uint32_t rdp_current = hfx_read_reg(0x04100008);

            sprintf(print_buf, "Start 0x%lx, end 0x%lx, current 0x%lx", rdp_start, rdp_end, rdp_current);
            graphics_draw_text(state->display, 0, 0, print_buf);

            if(rdp_current >= (rdp_start+BUFFER_SIZE_BYTES))
            {
                graphics_draw_text(state->display, 0, 100, "RDP command buffer overflowed!");
            }

            hfx_swap_buffers(state);
        }
    }
}