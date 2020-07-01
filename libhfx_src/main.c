#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#include <libdragon.h>
#include <rsp.h>
#include <rdp.h>

#include "hfx.h"

#define RB_SIZE 1024

extern const void __ucode_data_start;
extern const void __ucode_start;
extern const void __ucode_end;

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static volatile uint32_t rb[RB_SIZE/4] __attribute__((aligned(8)));
static uint32_t rb_start = 0, rb_end = 0;

static uint32_t array[4] __attribute__((aligned(8))) = {1,2,3,4};
static uint32_t array2[4] __attribute__((aligned(8))) = {5,6,7,8};
static uint32_t buffer[1024] __attribute__((aligned(8)));

static int done = 1;

static void rsp_done() 
{
    done = 0;
    printf("RSP status 0x%lx\n", *((volatile uint32_t*)(0xA4040010)));
}

static void queue_cmd(uint32_t cmd)
{
    rb[rb_end/4] = cmd;
    rb_end = (rb_end + 4) & 0x3FF; // TODO replace mask with one calculate from RB_SIZE
}

static void send_cmd()
{
    /* flush cache to make sure all queue operations are written to memory */
    data_cache_hit_writeback_invalidate(rb, RB_SIZE);
    
    printf("Write RB_END %lu\n", rb_end);
    /* Update RB_END pointer directly in RSP DMEM */
    *((volatile uint32_t*)(0xA4000000+HFX_REG_RB_END)) = rb_end;
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
    rsp_init();
    
    /* Attach SP handler and enable interrupt */
    register_SP_handler(&rsp_done);
    set_SP_interrupt(1);

    /* Load microcode for RSP */
    unsigned long data_size = (unsigned long) (&__ucode_start - &__ucode_data_start);
    unsigned long ucode_size = (unsigned long) (&__ucode_end - &__ucode_start);
    
    uint32_t *data_ptr = (uint32_t*)&__ucode_data_start;
    data_ptr[HFX_REG_RB_ADDR/4] = (uint32_t)&rb;
    data_ptr[HFX_REG_RB_SIZE/4] = RB_SIZE;
    data_cache_hit_writeback_invalidate(data_ptr, data_size);
    
    load_data((void*)&__ucode_data_start, data_size);
    load_ucode((void*)&__ucode_start, ucode_size);
    
    run_ucode();
    
    queue_cmd(HFX_CMD_DMA_READ);
    queue_cmd(2048);
    queue_cmd((uint32_t)&array);
    queue_cmd(sizeof(array)-1);
    queue_cmd(HFX_CMD_INT);
    
    queue_cmd(HFX_CMD_DMA_READ);
    queue_cmd(2048);
    queue_cmd((uint32_t)&array2);
    queue_cmd(sizeof(array2)-1);
    queue_cmd(HFX_CMD_INT);
    send_cmd();
    
    
    printf("Running %lu %lu %lu\n", rb[0], rb_end, data_size);
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
            
            *(volatile uint32_t *) 0xA4080000 = *(volatile uint32_t *) 0xA4080000; // TODO figure out what the heck is going on here
            *((volatile uint32_t*)(0xA4040010)) = 0x00000005;
        }
    }
}
