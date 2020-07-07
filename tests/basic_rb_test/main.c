#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#include <libdragon.h>
#include <hfx.h>
#include <hfx_rb.h>
#include <hfx_cmds.h>
#include <hfx_int.h>
#include <rsp.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static uint32_t array[4] __attribute__((aligned(8))) = {1,2,3,4};
static uint32_t array2[4] __attribute__((aligned(8))) = {5,6,7,8};
static uint32_t buffer[1024] __attribute__((aligned(8)));

static int done = 1;

static void rsp_done()
{
    done = 0;
    printf("RSP status 0x%lx\n", *((volatile uint32_t*)(0xA4040010)));
}

int main(void)
{
     /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    console_init();
    console_set_render_mode(RENDER_MANUAL);
    console_clear();

    hfx_state *state = hfx_init();
    hfx_register_rsp_int(state, rsp_done);

    hfx_cmd_dma_read_to_rsp(state, 2048, &array, sizeof(array));
    hfx_cmd_int(state);
    hfx_cmd_dma_read_to_rsp(state, 2048, &array2, sizeof(array));
    hfx_cmd_int(state);
    hfx_rb_submit(state);

        
    printf("Running\n");
    console_render();

    while(1)
    {
        if(done == 0)
        {
            done = 1;
            read_data(buffer, 4096);
            printf("Buffer values %lu %lu %lu %lu\n", buffer[512], buffer[513], buffer[514], buffer[515]);
            printf("Pointers are %lu %lu\n", buffer[HFX_REG_RB_ADDR/4], buffer[HFX_REG_RB_START/4]);
            printf("PC value 0x%lx\n", *(volatile uint32_t *) 0xA4080000);
            console_render();
            
            hfx_restart_rsp(state);
        }
    }
}