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

#define RGBA8_TO_RGBA5551(r,g,b,a) ((((uint32_t)(r)&0xf8u)<<8u) | (((uint32_t)(g)&0xf8u)<<4u) | (((uint32_t)(b)&0xf8u)>>2u) | ((uint32_t)(a)&0x01u))

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;

static int done = 1;
static hfx_state *state;

float v1[] =
{
    0.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

float t1[] =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};

uint8_t vc1[] =
{
    255.0f, 255.0f, 255.0f, 1.0f,
    255.0f, 255.0f, 255.0f, 1.0f,
    255.0f, 255.0f, 255.0f, 1.0f,
    255.0f, 255.0f, 255.0f, 1.0f,
    255.0f, 255.0f, 255.0f, 1.0f,
    255.0f, 255.0f, 255.0f, 1.0f,
};

static uint16_t tex_data[16*16] __attribute__((aligned(64)));

void load_tex_dat()
{
    for(int j=0; j < 16; j++)
    {
        for(int i=0; i < 16; i++)
        {

            if((i+j)%2 == 0)
            {
                tex_data[j*16 + i] = RGBA8_TO_RGBA5551(0xffu,0xffu,0xffu,0xffu);
            }
            else
            {
                if(i%3 == 0)
                {
                    tex_data[j*16 + i] = RGBA8_TO_RGBA5551(0xffu,0x00u,0x00u,0xffu);
                } 
                else if(i%3 == 1)
                {
                    tex_data[j*16 + i] = RGBA8_TO_RGBA5551(0x00u,0xffu,0x00u,0xffu);
                }
                else
                {
                    tex_data[j*16 + i] = RGBA8_TO_RGBA5551(0x00u,0x00u,0xffu,0xffu);
                }
            }
        }
    }

    data_cache_hit_writeback_invalidate(tex_data, sizeof(tex_data));
}

void exception(exception_t *data)
{
    hfx_fatal_error(state, "exception");
}

int main(void)
{
    /* enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    display_init( res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    register_exception_handler(exception);

    load_tex_dat();

    state = hfx_init();

    hfx_enable(state, HFX_TEXTURE_2D);

    hfx_tex_image_2d(state, 0, 0, HFX_RGBA, 16, 16, 0, HFX_RGBA, HFX_UNSIGNED_SHORT_5_5_5_1, tex_data);

    hfx_vertex_pointer(state, 3, HFX_FLOAT, 0, v1);
    hfx_color_pointer(state, 4, HFX_UNSIGNED_BYTE, 0, vc1);  
    hfx_tex_coord_pointer(state, 2, HFX_FLOAT, 0, t1);

    hfx_clear_color_f(state, 0.2f, 0.0f, 0.9f, 1.0f);

    float angle = 0.0;
    while(1)
    {
        if(angle == 360)
            angle = 0;
        else
            angle += 1.0f;
        done = 1;

        // Queue the next frame up
        hfx_clear(state, HFX_COLOR_BUFFER_BIT|HFX_DEPTH_BUFFER_BIT);
        hfx_load_identity(state);
        hfx_rotate_f(state, angle, 0, 0, 1);
        hfx_draw_arrays(state, HFX_TRIANGLES, 0, 6);

        hfx_swap_buffers(state);
    }
}
