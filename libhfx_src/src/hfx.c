#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <libdragon.h>
#include <stdio.h>
#include <stdbool.h>
#include "hfx_rdp.h"
#include "system.h"

#define RB_SIZE 1024

#define RDP_TIMEOUT (1*1000*1000)

#define COUNT_PER_US (COUNTS_PER_SECOND / 1000 / 1000)
#define HFX_DEBUG

extern const void _hfx_ucode_data_start;
extern const void _hfx_ucode_start;
extern const void _hfx_ucode_end;

static hfx_state state __attribute__((aligned(64)));

static char pbuf[256];

static volatile bool done;

static int __console_write( char *buf, unsigned int len )
{
    /* Write to special IO register in cen64 EMU */
    for(int x = 0; x < len; x++)
    {
        hfx_write_reg(0x18000004, (uint32_t)buf[x]);
    }

    /* Always write all */
    return len;
}

static stdio_t console_calls = {
    0,
    __console_write,
    0
};

void hfx_rdp_int()
{
    done = true;
}

void hfx_rsp_int()
{
    hfx_fatal_error(&state);
}

hfx_state *hfx_init()
{
    state.rb_start = 0;
    state.rb_end = 0;
    state.rb_size = HFX_RB_SIZE; // TODO probably want to make this programmable

    /* Initalize variables in hfx state */
    hfx_rb_calc_size_mask(&state);

    /* Initalize capabilties */
    hfx_init_caps(&state);
    hfx_set_mode(&state);
    hfx_set_scissor(&state, 0, 0, 320<<2, 240<<2);

    /* For now hardcode display dimensions to 320x240 */
    state.display_dim.width = 320;
    state.display_dim.height = 240;
    
    /* Initalize RSP */
    rsp_init();
    register_SP_handler(hfx_rsp_int);
    set_SP_interrupt(1);

    /* Setup RDP interrupt for library */
    register_DP_handler(hfx_rdp_int);
    set_DP_interrupt(1);
    
    /* Set RB pointer and size in ucode data */
    uint32_t data_size = (uint32_t) (&_hfx_ucode_start - &_hfx_ucode_data_start);
    uint32_t ucode_size = (uint32_t) (&_hfx_ucode_end - &_hfx_ucode_start);
    
    uint32_t *data_ptr = (uint32_t*)&_hfx_ucode_data_start;
    data_ptr[HFX_REG_RB_ADDR/4] = (uint32_t)&state.rb;
    data_ptr[HFX_REG_RB_SIZE/4] = HFX_RB_SIZE;
    data_cache_hit_writeback_invalidate(data_ptr, data_size);
    
    load_data((void*)&_hfx_ucode_data_start, data_size);
    load_ucode((void*)&_hfx_ucode_start, ucode_size);
    
    run_ucode();


    hfx_cmd_register_display(&state);

    HFX_UNUSED(console_calls);
#ifdef HFX_DEBUG
    /* Register ourselves with newlib */
    hook_stdio_calls( &console_calls );
#endif

    return &state;
}


void hfx_write_reg(uint32_t addr, uint32_t data)
{
    *((volatile uint32_t*)(0xA0000000|addr)) = data;
}

uint32_t hfx_read_reg(uint32_t addr)
{
    return *((volatile uint32_t*)(0xA0000000|addr));
}

void hfx_register_rsp_int(hfx_state *state, void *func_ptr)
{
    /* Attach the RSP intterupt handler */
    register_SP_handler(func_ptr);
    set_SP_interrupt(1);
}

void hfx_restart_rsp(hfx_state *state)
{
    hfx_write_reg(HFX_VADDR_REG_RSP_PC, hfx_read_reg(HFX_VADDR_REG_RSP_PC));
    hfx_write_reg(HFX_VADDR_REG_RSP_STATUS, HFX_RSP_CLEAR_BROKE|HFX_RSP_CLEAR_HALT);
}

void hfx_wait_us(uint64_t num_us)
{
    wait_ticks(num_us*COUNT_PER_US);
}

void hfx_wait_for_idle(hfx_state *state)
{
    uint64_t cmds[1];
    done = false;
    cmds[0] = HFX_RDP_PKT_SYNC_FULL;
    hfx_cmd_rdp(state, sizeof(cmds)/sizeof(uint64_t), cmds);
    hfx_rb_submit(state);

    uint32_t count = 0;
    while(done != true)
    {
        count += 1;
        if(count > RDP_TIMEOUT)
        {
            hfx_fatal_error(state);
        }
        hfx_wait_us(1);
    }
}

void hfx_fatal_error(hfx_state *state)
{
    // TODO replace this with proper define
    uint32_t rdp_status = hfx_read_reg(0x0410000C);
    uint32_t rsp_fifo_status = hfx_read_reg(HFX_VADDR_REG_STATUS);
    uint32_t rsp_status = hfx_read_reg(0x04040010);

    uint32_t rdp_start = hfx_read_reg(0x04100000);
    uint32_t rdp_end = hfx_read_reg(0x04100004);
    uint32_t rdp_current = hfx_read_reg(0x04100008);

    /* HALT RSP & RDP */
    // TODO add defines for these registers
    hfx_write_reg(0x04040010, 0x00000002);
    hfx_write_reg(0x0410000c, 0x0008);

    graphics_fill_screen(state->last_display, 0);
    graphics_draw_text(state->last_display, 10, 10, "FATAL ERROR!");

    sprintf(pbuf, "RSP STATUS: 0x%lx", rsp_status);
    graphics_draw_text(state->last_display, 10, 20, pbuf);

    sprintf(pbuf, "RSP FIFO STATUS: 0x%lx", rsp_fifo_status);
    graphics_draw_text(state->last_display, 10, 30, pbuf);

    sprintf(pbuf, "RDP STATUS: 0x%lx", rdp_status);
    graphics_draw_text(state->last_display, 10, 40, pbuf);

    sprintf(pbuf, "RSP FIFO\nStart 0x%08lx, end 0x%08lx", hfx_read_reg(HFX_VADDR_REG_RB_START), state->rb_end);
    graphics_draw_text(state->last_display, 10, 50, pbuf);

    sprintf(pbuf, "RSP CUR END 0x%lx\n", hfx_read_reg(HFX_VADDR_REG_RB_END));
    graphics_draw_text(state->last_display, 10, 70, pbuf);

    sprintf(pbuf, "RDP FIFO\nStart 0x%lx, end 0x%lx, current 0x%lx", rdp_start, rdp_end, rdp_current);
    graphics_draw_text(state->last_display, 10, 80, pbuf);

    while(1)
    {
        /* Do nothing */
    }
}