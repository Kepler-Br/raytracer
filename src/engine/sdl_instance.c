#include "sdl_instance.h"

static void init(t_sdl_instance *this, const char * const title)
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s: %s", "Cannot init SDL2", SDL_GetError());
        exit(0);
    }
    this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    this->resolution.x, this->resolution.y, SDL_WINDOW_SHOWN);
    if(this->window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s: %s", "Cannot create SDL2 window", SDL_GetError());
        exit(0);
    }
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_SOFTWARE);
    if(this->renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s: %s", "Cannot create SDL2 renderer", SDL_GetError());
        exit(0);
    }
}

static void present(t_sdl_instance *this)
{
    SDL_RenderPresent(this->renderer);
}

t_sdl_instance *construct_sdl_instance(const t_ivec2 resolution, const char * const title)
{
    t_sdl_instance *this;

    SDL_assert((this = malloc(sizeof(t_sdl_instance))) != NULL);
    this->init = &init;
    this->present = &present;

    this->resolution = resolution;
    this->init(this, title);
    return (this);
}

void destruct_sdl_instance(t_sdl_instance *sdl_instance)
{
	SDL_DestroyRenderer(sdl_instance->renderer);
	SDL_DestroyWindow(sdl_instance->window);
	SDL_Quit();
}