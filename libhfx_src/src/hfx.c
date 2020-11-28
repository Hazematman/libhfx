#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <libdragon.h>
#include <stdio.h>
#include "system.h"

#define RB_SIZE 1024
#define HFX_DEBUG

extern const void _hfx_ucode_data_start;
extern const void _hfx_ucode_start;
extern const void _hfx_ucode_end;

static hfx_state state __attribute__((aligned(8)));

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

hfx_state *hfx_init()
{
    state.rb_start = 0;
    state.rb_end = 0;
    state.rb_size = HFX_RB_SIZE; // TODO probably want to make this programmable

    /* Initalize variables in hfx state */
    hfx_rb_calc_size_mask(&state);

    /* For now hardcode display dimensions to 320x240 */
    state.display_dim.width = 320;
    state.display_dim.height = 240;
    
    /* Initalize RSP */
    rsp_init();
    
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