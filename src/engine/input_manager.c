#include "input_manager.h"

void update(struct s_input_manager *this)
{
    SDL_Event event;
    t_ivec2 mouse_move;

//    SDL_GetMouseState(&mouse_move.x, &mouse_move.y);
//    SDL_GetGlobalMouseState(&mouse_move.x, &mouse_move.y);
//    this->mouse_delta = ivec2_ivec2_sub(&mouse_move, &this->current_mouse);
    this->current_mouse = mouse_move;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_WINDOWEVENT)
            if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                this->is_close_event_pending = 1;
        if(event.type == SDL_KEYDOWN)
        {
            t_map_item map_item;
            uint key = event.key.keysym.sym;
            int value = 1;
            map_item.key_size = sizeof(int);
            map_item.value_size = sizeof(uint);
            map_item.value = (void *)&value;
            map_item.key = (void *)&key;
            this->map->emplace(this->map, map_item);
        }
        if(event.type == SDL_KEYUP)
        {
            t_map_item map_item;
            uint key = event.key.keysym.sym;
            int value = 0;
            map_item.key_size = sizeof(int);
            map_item.value_size = sizeof(uint);
            map_item.value = (void *)&value;
            map_item.key = (void *)&key;
            this->map->emplace(this->map, map_item);
        }
        if(event.type == SDL_MOUSEMOTION)
		{
			this->mouse_delta.x = event.motion.xrel;
			this->mouse_delta.y = event.motion.yrel;
		}
    }
}

int is_key_down(struct s_input_manager *this, uint keycode)
{
    t_map_item *item;

    item = this->map->find(this->map, (void *)&keycode, sizeof(uint));
    if(item == NULL)
        return (0);
    if(*(int*)item->value == 0)
        return (0);
    else
        return (1);
}

t_input_manager *construct_input_manager(t_sdl_instance *sdl_instance)
{
    t_input_manager *this;

    SDL_assert((this = malloc(sizeof(t_input_manager))) != NULL);
    this->sdl_instance = sdl_instance;
    this->map = ft_make_t_map();
    this->current_mouse = (t_ivec2){{0.0f, 0.0f}};
    this->mouse_delta = (t_ivec2){{0.0f, 0.0f}};
    this->is_close_event_pending = 0;
    this->update = &update;
    this->is_key_down = &is_key_down;
    return (this);
}

void destruct_input_manager(t_input_manager *this)
{
	ft_destroy_t_map(this->map);
}