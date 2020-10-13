#ifndef HFX_TYPES_H
#define HFX_TYPES_H
#include <libdragon.h>
#include <stdint.h>

#define HFX_RB_SIZE 1024

typedef struct hfx_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} hfx_color;

typedef struct hfx_state {
    uint32_t rb[HFX_RB_SIZE/4] __attribute__((aligned(8)));
    uint32_t rb_start;
    uint32_t rb_end;
    uint32_t rb_size;
    uint32_t rb_size_mask;
    display_context_t display;

    hfx_color clear_color;
    hfx_color vertex_color;

    struct
    {
        uint32_t width;
        uint32_t height;
    } display_dim;
} hfx_state __attribute__((aligned(8)));

#endif
