#include "framebuffer.h"

static float init(struct s_framebuffer *this, t_ivec2 resolution)
{
    this->resolution = resolution;
    if(this->pixels != NULL)
        free(this->pixels);
    this->pixel_count = resolution.x * resolution.y;
    this->pixels = malloc(this->pixel_count * sizeof(uint32_t));
    if(this->pixels == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s", "Cannot malloc pixel framebuffer");
        exit(0);
    }
    if(this->framebuffer != NULL)
        this->destroy(this);
    this->framebuffer = SDL_CreateTexture(this->sdl_instance->renderer,
                                          SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING,
                                          resolution.x, resolution.y);
    if(this->framebuffer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s: %s", "Cannot create SDL2 texture", SDL_GetError());
        exit(0);
    }
}

static void lock(t_framebuffer *this)
{
    static int pitch;

    this->is_locked = 1;
    if(SDL_LockTexture(this->framebuffer, NULL, (void**)&this->pixels, &pitch))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s: %s", "Cannot lock framebuffer", SDL_GetError());
        exit(0);
    }
}

static  void unlock(t_framebuffer *this)
{
    this->is_locked = 0;
    SDL_UnlockTexture(this->framebuffer);
}

void destroy(struct s_framebuffer *this)
{
    if(this->framebuffer != NULL)
      SDL_DestroyTexture(this->framebuffer);
    this->framebuffer = NULL;
}

static void put(struct s_framebuffer *this)
{
    if(this->is_locked)
    {
        SDL_LogInfo(SDL_LOG_CATEGORY_RENDER, "%s", "Cannot put locked buffer.");
        return;
    }
    int i = SDL_RenderCopy(this->sdl_instance->renderer, this->framebuffer, NULL, NULL);
//    printf("%d\n", i);
//    SDL_LogError(SDL_LOG_CATEGORY_ASSERT, "%s: %s", "Cannot lock framebuffer", SDL_GetError());
}

t_framebuffer construct_framebuffer(const t_ivec2 resolution, t_sdl_instance *sdl_instance)
{
    t_framebuffer this;

    this.pixels = NULL;
    this.framebuffer = NULL;
    this.sdl_instance = sdl_instance;

    this.init = &init;
    this.unlock = &unlock;
    this.lock = &lock;
    this.destroy = &destroy;
    this.put = &put;

    this.init(&this, resolution);
    return (this);
}
