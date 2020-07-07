#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <libdragon.h>

#define RB_SIZE 1024

extern const void __ucode_data_start;
extern const void __ucode_start;
extern const void __ucode_end;

static hfx_state state __attribute__((aligned(8)));

hfx_state *hfx_init()
{
    state.rb_start = 0;
    state.rb_end = 0;
    state.rb_size = HFX_RB_SIZE; // TODO probably want to make this programmable

    /* Initalize variables in hfx state */
    hfx_rb_calc_size_mask(&state);
    
    /* Initalize RSP */
    rsp_init();
    
    /* Set RB pointer and size in ucode data */
    uint32_t data_size = (uint32_t) (&__ucode_start - &__ucode_data_start);
    uint32_t ucode_size = (uint32_t) (&__ucode_end - &__ucode_start);
    
    uint32_t *data_ptr = (uint32_t*)&__ucode_data_start;
    data_ptr[HFX_REG_RB_ADDR/4] = (uint32_t)&state.rb;
    data_ptr[HFX_REG_RB_SIZE/4] = HFX_RB_SIZE;
    data_cache_hit_writeback_invalidate(data_ptr, data_size);
    
    load_data((void*)&__ucode_data_start, data_size);
    load_ucode((void*)&__ucode_start, ucode_size);
    
    run_ucode();


    hfx_cmd_register_display(&state);

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