#include "mainloop.h"
#include "shape.h"

static void run(t_mainloop *this)
{
    static const float step_time = 30.0f;
    float lag;

    lag = 30.0f;
    this->is_running = 1;
    this->check_timer->check(this->check_timer);
    while(this->is_running)
    {
        this->calc_deltatime(this);
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

        lag += this->check_timer->check(this->check_timer);
        this->sdl_instance->present(this->sdl_instance);
        this->frame_count++;
        if(this->frame_count % 60 == 0)
            printf("%f\n", 1.0f/this->deltatime);
        this->limit_fps(this);
    }
}

static void calc_deltatime(t_mainloop *this)
{
    static float last_time = 0;
    static float current_time = 0;

    if (current_time > last_time)
        last_time = current_time;
    current_time = SDL_GetTicks();
    this->deltatime = (current_time - last_time) / 1000.0f;
}

void limit_fps(struct s_mainloop *this)
{
    static uint32_t start = (uint)-1;

    if(start == (uint32_t)-1)
        start = SDL_GetTicks();
    if ((uint32_t)(1000 / this->max_fps) > (uint32_t)(SDL_GetTicks() - start))
        SDL_Delay(1000 / this->max_fps - (SDL_GetTicks() - start));
    start = SDL_GetTicks();
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
    this->framebuffer->lock(this->framebuffer);
}

static void render(t_mainloop *this)
{
    uint32_t *pixel;
    t_shape *plane = construct_shape_plane((t_vec3){{0.0f, 0.0f, 0.0f}}, (t_vec3){{0.0f, 1.0f, 0.0f}});

    for(int x = 0; x < this->framebuffer->resolution.x; x++)
    {
        for(int y = 0; y < this->framebuffer->resolution.y; y++)
        {
            pixel = this->framebuffer->get_pixel(this->framebuffer, (t_ivec2){{x, y}});
            t_vec2 screen_coord = (t_vec2){{2.0f * x / this->framebuffer->resolution.x - 1.0f,
                                            2.0f * y / this->framebuffer->resolution.y - 1.0f}};
            t_ray ray = this->camera->make_ray(this->camera, &screen_coord);
            t_intersection intersection = construct_intersection(ray);
            if(plane->intersect(plane, &intersection))
                *pixel = 0xFFFFFF;
            else
                *pixel = 0x0;
//            *pixel = 0xFF00FF;
        }
    }

//    for(int i = 0; i < (int)this->framebuffer->pixel_count; i++)
//        if(i%1 == 0)
//            this->framebuffer->pixels[i] = rand()%0xFFFFFF;
}

static void postrender(t_mainloop *this)
{
    this->framebuffer->unlock(this->framebuffer);
    this->framebuffer->put(this->framebuffer);
}

void input(t_mainloop *this)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch( event.type ){
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					this->is_running = 0;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
					this->is_running = 0;
				break;
			default:
				break;
		}
	}
}

void stop(struct s_mainloop *this)
{
    this->is_running = 0;
}


t_mainloop *construct_mainloop(t_ivec2 resolution, const char * const title)
{
    t_mainloop *this;

    SDL_assert((this = malloc(sizeof(t_mainloop))) != NULL);

    this->run = &run;
    this->calc_deltatime = &calc_deltatime;
    this->update = &update;
    this->late_update = &late_update;
    this->fixed_update = &fixed_update;
    this->prerender = &prerender;
    this->render = &render;
    this->postrender = &postrender;
    this->input = &input;
    this->stop = &stop;
    this->limit_fps = &limit_fps;

    this->deltatime = 1.0f;
    this->frame_count = 0;
    this->max_fps = 60;

    this->check_timer = construct_check_timer();
    this->sdl_instance = construct_sdl_instance(resolution, title);
    this->framebuffer = construct_framebuffer(resolution, this->sdl_instance);
    this->camera = construct_camera((t_vec3){{1.0f, 5.0f, 2.0f}}, (t_vec3){{0.0f, 0.0f, 0.0f}}, (t_vec3){{0.0f, 1.0f, 0.0f}}, M_PI/4.0f, this->framebuffer->resolution.x/this->framebuffer->resolution.y);
    return (this);
}
