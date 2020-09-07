#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <SDL2/SDL.h>
#include "sdl_instance.h"
#include "ivec2.h"
#include "vec3.h"

typedef struct s_framebuffer
{
    cl_char *pixels;
    t_ivec2 resolution;
    SDL_Texture *framebuffer;
    t_sdl_instance *sdl_instance;
    size_t pixel_count;
    size_t color_count;
    int is_locked;

    void (*init)(struct s_framebuffer *this, t_ivec2 resolution);
    void (*lock)(struct s_framebuffer *this);
    void (*unlock)(struct s_framebuffer *this);
    void (*destroy)(struct s_framebuffer *this);
    void (*put)(struct s_framebuffer *this);

} t_framebuffer;

t_framebuffer	*construct_framebuffer(const t_ivec2 resolution, t_sdl_instance *sdl_instance);
void			destruct_framebuffer(t_framebuffer *framebuffer);

#endif // FRAMEBUFFER_H
