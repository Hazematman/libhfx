#ifndef HFX_TYPES_H
#define HFX_TYPES_H
#include <SDL2/SDL.h>

typedef struct hfx_state {
    SDL_Window *window;
    SDL_GLContext context;
} hfx_state;

#endif
