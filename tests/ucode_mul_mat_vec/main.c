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

    float vec[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float mat[16] = {2.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 2.0f, 0.0f, 0.0f,
                     2.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 2.0f, 0.0f, 2.0f};

    for(int i=0; i < 2; i++)
    {
        uint32_t ivalue1 = hfx_float_to_fixed(vec[i*2 + 0]);
        uint32_t ivalue2 = hfx_float_to_fixed(vec[i*2 + 1]);
        printf("ivalues 0x%08X 0x%08X\n", (unsigned int)ivalue1, (unsigned int)ivalue2); 
        write_dmem(0*4 + i, ivalue1>>16, ivalue2>>16);
        write_dmem(1*4 + i, ivalue1, ivalue2);
    }

    for(int j=0; j < 4; j++)
    {
        for(int i=0; i < 2; i++)
        {
            uint32_t ivalue1 = hfx_float_to_fixed(mat[j*4+i*2+0]);
            uint32_t ivalue2 = hfx_float_to_fixed(mat[j*4+i*2+1]);
            write_dmem((j*2+2+0)*4+i, ivalue1>>16, ivalue2>>16);
            write_dmem((j*2+2+1)*4+i, ivalue1, ivalue2);
            printf("writing 0x%08X 0x%08X, to %d %d\n", ivalue1, ivalue2, (j*2+2+0)*4+i, (j*2+2+1)*4+i);
        }
    }

    printf("Running!\n");
    console_render();

    run_ucode();

    while(1)
    {
        if(done == 0)
        {
            done = 1;

            uint32_t ivalue[4];
            float fvalue[4];
            printf("Results are:\n");
            for(int i=0; i < 4; i++)
            {
                ivalue[i] = read_dmem(i);
                fvalue[i] = ivalue[i] / 65536.0f;
                printf("%d: int 0x%08X float %f\n", i, (unsigned int)ivalue[i], fvalue[i]);
            }

            console_render();
        }
    }
}
