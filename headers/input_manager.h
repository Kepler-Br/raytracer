#ifndef RAYTRACER_INPUT_MANAGER_H
#define RAYTRACER_INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include "sdl_instance.h"
#include "ivec2.h"
#include "libft.h"

typedef struct s_input_manager
{
    t_sdl_instance *sdl_instance;
    t_map *map;
    int is_close_event_pending;

    t_ivec2 mouse_delta;
    t_ivec2 current_mouse;

    void (*update)(struct s_input_manager *this);
    int (*is_key_down)(struct s_input_manager *this, uint keycode);
} t_input_manager;

t_input_manager *construct_input_manager(t_sdl_instance *sdl_instance);

#endif //RAYTRACER_INPUT_MANAGER_H
