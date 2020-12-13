#include <hfx.h>
#include <hfx_types.h>
#include <hfx_int.h>
#include <hfx_rb.h>
#include <stdbool.h>

void hfx_init_caps(hfx_state *state)
{
    state->caps.dirty = true;

    state->caps.depth_test = false;
    state->caps.vertex_array = false;
    state->caps.color_array = false;
    state->caps.texture_2d = false;
}

void hfx_enable(hfx_state *state, uint32_t cap)
{
    bool *value = NULL;
    switch(cap)
    {
        case HFX_DEPTH_TEST:
            value = &state->caps.depth_test;
            break;
        case HFX_VERTEX_ARRAY:
            value = &state->caps.vertex_array;
            break;
        case HFX_COLOR_ARRAY:
            value = &state->caps.color_array;
            break;
        case HFX_TEXTURE_2D:
            value = &state->caps.texture_2d;
        default:
            break;
    }

    if(value != NULL && *value != true)
    {
        *value = true;
        state->caps.dirty = true;
    }
}