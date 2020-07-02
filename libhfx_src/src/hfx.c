#include <hfx.h>
#include <hfx_types.h>
#include <rsp.h>

#define RB_SIZE 1024

extern const void __ucode_data_start;
extern const void __ucode_start;
extern const void __ucode_end;

static hfx_state state;

void hfx_init()
{
    state.rb_start = 0;
    state.rb_end = 0;
    
    /* Initalize RSP */
    rsp_init();
    
    /* Set RB pointer and size in ucode data */
    uint32_t data_size = (uint32_t) (&__ucode_start - &__ucode_data_start);
    uint32_t ucode_size = (uint32_t) (&__ucode_end - &__ucode_start);
    
    uint32_t *data_ptr = (uint32_t*)&__ucode_data_start;
    data_ptr[HFX_REG_RB_ADDR/4] = (uint32_t)&rb;
    data_ptr[HFX_REG_RB_SIZE/4] = HFX_RB_SIZE;
    data_cache_hit_writeback_invalidate(data_ptr, data_size);
    
    load_data((void*)&__ucode_data_start, data_size);
    load_ucode((void*)&__ucode_start, ucode_size);
    
    run_ucode();
}
