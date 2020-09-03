#include "mainloop.h"

static void run(t_mainloop *this)
{
    const static float step_time = 30.0f;
    float lag;

    lag = 30.0f;
    this->is_running = 1;
    this->check_timer.check(&this->check_timer);
    while(this->is_running)
    {

        this->prerender(this);
        this->render(this);
        this->postrender(this);

        this->update(this);
        this->late_update(this);
        while(lag > step_time)
        {
            this->input(this);
            this->fixed_update(this);
            lag -= step_time;
        }

        lag += this->check_timer.check(&this->check_timer);
        this->sdl_instance.present(&this->sdl_instance);
//        this->stop(this);
    }
}

static void prepare(t_mainloop *this)
{

}

static void update(t_mainloop *this)
{

}

static void late_update(t_mainloop *this)
{

}

static void fixed_update(t_mainloop *this)
{

}

static void prerender(t_mainloop *this)
{
    this->framebuffer.lock(&this->framebuffer);
}

static void render(t_mainloop *this)
{
    this->framebuffer.pixels[10000] = 0xFF00FF;
    SDL_SetRenderDrawColor(this->sdl_instance.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->sdl_instance.renderer);
    SDL_SetRenderDrawColor(this->sdl_instance.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(this->sdl_instance.renderer, 100, 100, 200, 200);
}

static void postrender(t_mainloop *this)
{
    this->framebuffer.unlock(&this->framebuffer);
    this->framebuffer.put(&this->framebuffer);
}

void input(t_mainloop *this)
{

}

void stop(struct s_mainloop *this)
{
    this->is_running = 0;
}


t_mainloop construct_mainloop(t_ivec2 resolution, const char * const title)
{
    t_mainloop this;

    this.run = &run;
    this.prepare = &prepare;
    this.update = &update;
    this.late_update = &late_update;
    this.fixed_update = &fixed_update;
    this.prerender = &prerender;
    this.render = &render;
    this.postrender = &postrender;
    this.input = &input;
    this.stop = &stop;

    this.check_timer = construct_check_timer();
    this.sdl_instance = construct_sdl_instance(resolution, title);
    this.framebuffer = construct_framebuffer(resolution, &this.sdl_instance);
    return (this);
}
