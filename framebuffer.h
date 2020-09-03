#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <SDL2/SDL.h>
#include "sdl_instance.h"
#include "linear_alg.h"

typedef struct s_framebuffer
{
    uint32_t *pixels;
    t_ivec2 resolution;
    SDL_Texture *framebuffer;
    t_sdl_instance *sdl_instance;
    size_t pixel_count;
    int is_locked;

    void (*init)(struct s_framebuffer *this, t_ivec2 resolution);
    void (*lock)(struct s_framebuffer *this);
    void (*unlock)(struct s_framebuffer *this);
    void (*destroy)(struct s_framebuffer *this);
    void (*put)(struct s_framebuffer *this);
    uint32_t *(*get_pixel)(struct s_framebuffer *this, t_ivec2 position);

} t_framebuffer;

t_framebuffer *construct_framebuffer(const t_ivec2 resolution, t_sdl_instance *sdl_instance);

#endif // FRAMEBUFFER_H
