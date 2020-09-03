#ifndef SDL_INSTANCE_H
#define SDL_INSTANCE_H

#include "linear_alg.h"
#include <SDL2/SDL.h>

typedef struct s_sdl_instance
{
    t_ivec2 resolution;
    SDL_Window *window;
    SDL_Renderer *renderer;

    void (*init)(struct s_sdl_instance *this, const char * const title);
    void (*present)(struct s_sdl_instance *this);
} t_sdl_instance;

t_sdl_instance *construct_sdl_instance(const t_ivec2 resolution, const char * const title);

#endif // SDL_INSTANCE_H
