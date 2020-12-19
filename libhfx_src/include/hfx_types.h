#ifndef HFX_TYPES_H
#define HFX_TYPES_H
#include <libdragon.h>
#include <stdint.h>
#include <stdbool.h>

#define HFX_RB_SIZE 1024

typedef struct hfx_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} hfx_color;

typedef struct hfx_tex_info {
    uint32_t width;
    uint32_t height;
    uint32_t type;
} hfx_tex_info;

struct hfx_state {
    uint32_t rb[HFX_RB_SIZE/4] __attribute__((aligned(8)));
    uint32_t rb_start;
    uint32_t rb_end;
    uint32_t rb_size;
    uint32_t rb_size_mask;
    display_context_t display;
    display_context_t last_display;
    void *display_ptr;

    struct
    {
        bool depth_test;
        bool vertex_array;
        bool color_array;
        bool texture_2d;
        bool dirty;
    } caps;

    hfx_tex_info default_tex_info;
    hfx_tex_info *cur_tex;

    uint64_t rdp_mode;

    hfx_color clear_color;
    hfx_color vertex_color;

    float model_matrix[16];

    float *vertex_pointer;
    uint32_t vertex_size;

    uint8_t *color_pointer;
    uint32_t color_size;

    float *tex_coord_pointer;
    uint32_t tex_coord_size;

    struct
    {
        uint32_t width;
        uint32_t height;
    } display_dim;
};

#endif
