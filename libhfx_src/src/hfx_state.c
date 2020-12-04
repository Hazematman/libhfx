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
}

void hfx_enable(hfx_state *state, uint32_t cap)
{
    switch(cap)
    {
        case HFX_DEPTH_TEST:
            state->caps.depth_test = true;
        case HFX_VERTEX_ARRAY:
            state->caps.vertex_array = true;
        case HFX_COLOR_ARRAY:
            state->caps.color_array = true;
        default:
            break;
    }
}