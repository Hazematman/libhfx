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

    float v1[4], v2[4], v3[4];
    float vc1[4], vc2[4], vc3[4];

    v1[0] = 0.0f;
    v1[1] = 0.0f;
    v1[2] = 0.0f;
    v1[3] = 1.0f;
    
    v2[0] = 100.0f;
    v2[1] = 0.0f;
    v2[2] = 0.0f;
    v2[3] = 1.0f;

    v3[0] = 100.0f / 2.0f;
    v3[1] = 100.0f;
    v3[2] = 0.0f;
    v3[3] = 1.0f;

    vc1[0] = 255.0f;
    vc1[1] = 0.0f;
    vc1[2] = 0.0f;
    vc1[3] = 255.0f;

    vc2[0] = 0.0f;
    vc2[1] = 255.0f;
    vc2[2] = 0.0f;
    vc2[3] = 255.0f;

    vc3[0] = 0.0f;
    vc3[1] = 0.0f;
    vc3[2] = 255.0f;
    vc3[3] = 255.0f;    

    // TODO this rdp command is only here to set the default clip
    // Should really be part of the hfx library
    hfx_cmd_rdp(state, sizeof(cmds1)/sizeof(uint64_t), cmds1);
    hfx_load_identity(state);
    //hfx_rotate_f(state, 50, 0, 0, 1);
    hfx_translate_f(state, 50.0f, 50.0f, 0.0f);

    hfx_color_f(state, 1.0f, 1.0f, 0.0f, 1.0f);
    hfx_draw_tri_f(state, v1, v2, v3, vc1, vc2, vc3);
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_rb_submit(state);

    hfx_clear_color_f(state, 0.0f, 0.0f, 0.0f, 1.0f);

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

            //angle = 240.0f;

            sprintf(pbuf, "Done %f", angle);
            graphics_draw_text(state->display, 0, 100, pbuf);

            sprintf(pbuf, "Start 0x%08lx, end 0x%08lx", hfx_read_reg(HFX_VADDR_REG_RB_START), state->rb_end);
            graphics_draw_text(state->display, 0, 130, pbuf);

            uint32_t rdp_start = hfx_read_reg(0x04100000);
            uint32_t rdp_end = hfx_read_reg(0x04100004);
            uint32_t rdp_current = hfx_read_reg(0x04100008);

            sprintf(pbuf, "Start 0x%lx, end 0x%lx, current 0x%lx", rdp_start, rdp_end, rdp_current);
            graphics_draw_text(state->display, 0, 160, pbuf);

            hfx_swap_buffers(state);

            // Queue the next frame up
            hfx_clear(state, HFX_COLOR_BUFFER_BIT);
            hfx_load_identity(state);
            hfx_translate_f(state, 100.0f, 100.0f, 0.0f);
            hfx_rotate_f(state, angle, 0, 0, 1);

            hfx_color_f(state, 1.0f, 1.0f, 0.0f, 1.0f);
            hfx_draw_tri_f(state, v1, v2, v3, vc1, vc2, vc3);
            hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
            hfx_rb_submit(state);
        }
    }
}