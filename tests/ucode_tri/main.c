#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include <hfx_int.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static volatile int done = 1;

extern const void _ucode_data_start;
extern const void _ucode_start;
extern const void _ucode_end;

void sp_handler()
{
    done = 0;
}

void write_dmem(uintptr_t offset, uint16_t value1, uint16_t value2)
{
    volatile uint32_t *dmem = (volatile uint32_t*)(0xa4000000);
    uint32_t value = (value1<<16) | value2;
    dmem[offset] = value;
}

uint32_t read_dmem(uintptr_t offset)
{
    volatile uint32_t *dmem = (volatile uint32_t*)(0xa4000000);
    return dmem[offset];
}

void read_dmem_two(uintptr_t offset, uint16_t *v1, uint16_t *v2)
{
    volatile uint32_t *dmem = (volatile uint32_t*)(0xa4000000);
    uint32_t val = dmem[offset];
    *v1 = val >> 16;
    *v2 = (uint16_t)val;
}

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    console_init();
    console_set_render_mode(RENDER_MANUAL);
    rsp_init();

    /* Attach SP handler and enable interrupt */
    register_SP_handler(&sp_handler);
    set_SP_interrupt(1);

    // Size must be multiple of 8 and start & end must be aligned to 8 bytes
    unsigned long data_size = (unsigned long) (&_ucode_start - &_ucode_data_start);
    unsigned long ucode_size = (unsigned long) (&_ucode_end - &_ucode_start);
    load_data((void*)&_ucode_data_start, data_size);
    load_ucode((void*)&_ucode_start, ucode_size);

    float x1 = 100.0f, y1 = 30.0f;
    float x2 = 120.0f, y2 = 100.0f;
    float x3 = 30.0f, y3 = 150.0f;

    uint32_t ix1 = hfx_float_to_fixed(x1), iy1 = hfx_float_to_fixed(y1);
    uint32_t ix2 = hfx_float_to_fixed(x2), iy2 = hfx_float_to_fixed(y2);
    uint32_t ix3 = hfx_float_to_fixed(x3), iy3 = hfx_float_to_fixed(y3);

    float exp_x1 = (x3-x1)/(y3-y1);
    float exp_x2 = (x2-x1)/(y2-y1);
    float exp_x3 = (x3-x2)/(y3-y2);

    float winding = (x1*y2 - x2*y1) + (x2*y3 - x3*y2) + (x3*y1 - x1*y3);

    write_dmem(0*4, ix1>>16, iy1>>16);
    write_dmem(1*4, ix1, iy1);

    write_dmem(2*4, ix2>>16, iy2>>16);
    write_dmem(3*4, ix2, iy2);

    write_dmem(4*4, ix3>>16, iy3>>16);
    write_dmem(5*4, ix3, iy3);

    printf("Running!\n");
    console_render();

    run_ucode();

    while(1)
    {
        if(done == 0)
        {
            done = 1;

            uint32_t dxHDy = read_dmem(5);
            uint32_t dxMDy = read_dmem(7);
            uint32_t dxLDy = read_dmem(3);

            x1 = ((float)(int)dxHDy) / 65536.0f;
            x2 = ((float)(int)dxMDy) / 65536.0f;
            x3 = ((float)(int)dxLDy) / 65536.0f;

            printf("Done!\n");
            printf("fixed 0x%X float %f expecting %f\n", (int)dxHDy, x1, exp_x1);
            printf("fixed 0x%X float %f expecting %f\n", (int)dxMDy, x2, exp_x2);
            printf("fixed 0x%X float %f expecting %f\n\n", (int)dxLDy, x3, exp_x3);

            for(int i=0; i < 4; i++)
            {
                uint32_t w1, w2;
                w1 = read_dmem((i*2 + 0));
                w2 = read_dmem((i*2 + 1));
                printf("0x%08X 0x%08X\n", w1, w2);
            }

            console_render();
        }
    }
}
