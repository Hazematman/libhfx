#ifndef HFX_TYPES_H
#define HFX_TYPES_H
#include <SDL2/SDL.h>

typedef struct hfx_state {
    SDL_Window *window;
    SDL_GLContext context;

    float model_matrix[16];
} hfx_state;

#endif
