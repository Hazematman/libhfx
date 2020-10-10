#include <hfx.h>
#include <hfx_types.h>
#include <GL/glew.h>
#include <stdio.h>

#define BITS_PER_PIXEL 8

static hfx_state prv_state;

static void checkSDLError()
{
    const char *errMsg = SDL_GetError();

    if(*errMsg != '\0') {
        printf("%s\n", errMsg);
    }
}

hfx_state *hfx_init()
{
    hfx_state *state = &prv_state;

    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        printf("Failed to init SDL\n");
    }
    checkSDLError();

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    state->window = SDL_CreateWindow("LibHFX",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            640, 480,
                            SDL_WINDOW_OPENGL);
    checkSDLError();
    state->context = SDL_GL_CreateContext(state->window);
    checkSDLError();

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("Failed to init glew\n");
    }

    return state;
}

void hfx_swap_buffers(hfx_state *state)
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            exit(0);
        }
    }

    SDL_GL_SwapWindow(state->window);
}

void hfxEnable(hfx_state *state, uint32_t cap)
{
    glEnable(cap);
}

void hfxClearColor(hfx_state *state, float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void hfxClear(hfx_state *state, uint32_t bits)
{
    glClear(bits);
}

void hfxVertexPointer(hfx_state *state, int32_t size, uint32_t type, uint32_t stride, void *pointer)
{
    glVertexPointer(size, type, stride, pointer);
}

void hfxColorPointer(hfx_state *state, int32_t size, uint32_t type, uint32_t stride, void *pointer)
{
    glColorPointer(size, type, stride, pointer);
}

void hfxLoadIdentity(hfx_state *state)
{
    hfx_load_identity(state);
}

void hfxTranslatef(hfx_state *state, float x, float y, float z)
{
    hfx_translate_f(state, x, y, z);
}

void hfxRotatef(hfx_state *state, float angle, float x, float y, float z)
{
    hfx_rotate_f(state, angle, x, y, z);
}

void hfxScalef(hfx_state *state, float x, float y, float z)
{
    hfx_scale_f(state, x, y, z);
}

void hfxDrawArrays(hfx_state *state, uint32_t mode, uint32_t first, uint32_t count)
{
    glLoadMatrixf(state->model_matrix);
    glDrawArrays(mode, first, count);
}