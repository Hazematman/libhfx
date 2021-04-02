#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <hfx_rdp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXTURE_ALIGN 64
#define ALIGN_64BYTE(x)     ((void *)(((uint32_t)(x)+63) & 0xFFFFFFC0))

void hfx_init_textures(hfx_state *state)
{
    state->tex_info.current_tex = 0;
    state->tex_info.num_texs = HFX_MIN_TEXTURES;
    state->tex_info.tex_list = (hfx_tex_info*)malloc(state->tex_info.num_texs*sizeof(hfx_tex_info));
    for(int i = 0; i < state->tex_info.num_texs; i++)
    {
        state->tex_info.tex_list[i].alloced = false;
        state->tex_info.tex_list[i].data = NULL;
    }

    /* Mark default texture 0 as already allocated */
    /* Required to be conformant with OpenGL */
    state->tex_info.tex_list[0].alloced = true;
}

void hfx_gen_textures(hfx_state *state, uint32_t n, uint32_t *textures)
{
    for(int i = 0; i < n; i++)
    {
        bool found = false;
        for(int j = 0; j < state->tex_info.num_texs; j++)
        {
            if(state->tex_info.tex_list[j].alloced == false)
            {
                found = true;
                textures[i] = j; 
            }
            
            // TODO if found is false realloc array
            // Right now only 16 textures can exist
            HFX_UNUSED(found);
        }
    }
}

void hfx_tex_image_2d(hfx_state *state, uint32_t target, int32_t level, int32_t internalformat, uint32_t width, uint32_t height, int32_t border, uint32_t format, uint32_t type, const void *data)
{
    int bpp = 2; // TODO Have function to get proper bit depth
    uint32_t size = width * height * bpp;
    hfx_tex_info *cur_tex = &state->tex_info.tex_list[state->tex_info.current_tex];

    cur_tex->unsafe_data = malloc(size + (TEXTURE_ALIGN));
    cur_tex->data = ALIGN_64BYTE(cur_tex->unsafe_data);

    memcpy(cur_tex->data, data, size);
    data_cache_hit_writeback_invalidate(cur_tex->data, size);

    cur_tex->width = width;
    cur_tex->height = height;
    cur_tex->type = HFX_UNSIGNED_SHORT_5_5_5_1;
    state->tex_info.dirty = true;
}
