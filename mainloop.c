#include "mainloop.h"
#include "shape.h"
#include "ray.h"

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
		this->time_since_start += this->deltatime;
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
        if(this->frame_count % ((int)(1.0f/this->deltatime) == 0 ? 1 : (int)(1.0f/this->deltatime)) == 0)
            printf("%d\n", (int)(1.0f/this->deltatime));
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
//	t_shape *sphere = this->scene->cached_shapes[1];
//	((t_shape_plane *)sphere->inhereted)->position.x = sin(this->time_since_start)*2.0f;
//	((t_shape_plane *)sphere->inhereted)->position.z = cos(this->time_since_start)*2.0f;

//	this->camera->look_at(this->camera, (t_vec3){{-5.0f, 1.0f, 0.0f}}, (t_vec3){{cos(this->time_since_start)*2.0f, 0.0f, 0.0f}}, (t_vec3){{0.0f, -1.0f, 0.0f}});
}

static void prerender(t_mainloop *this)
{
    this->framebuffer->lock(this->framebuffer);
}

static void render(t_mainloop *this)
{
    int x;
    int y;

    for(x = 0; x < this->framebuffer->resolution.x; x++)
    {
        for(y = 0; y < this->framebuffer->resolution.y; y++)
        {
            if(this->render_mask[x * y] == 0)
                continue;
            t_vec2 screen_coord = (t_vec2){{2.0f * x / this->framebuffer->resolution.x - 1.0f,
                                            2.0f * y / this->framebuffer->resolution.y - 1.0f}};
            t_ray ray = this->camera->make_ray(this->camera, &screen_coord);
            t_intersection intersection = construct_intersection(ray);
            if(this->scene->intersect(this->scene, &intersection))
            {
				t_vec3 result_color;
//            	result_color = (t_vec3){{intersection.shape->absorb_color.x + this->scene->ambient_color.x, intersection.shape->absorb_color.y + this->scene->ambient_color.y, intersection.shape->absorb_color.z + this->scene->ambient_color.z}};


                float dist;
                if(this->scene->is_point_visible(this->scene, intersection.position(&intersection), this->scene->cached_point_lights[0]->position, &dist))
				{
                	t_vec3 light_color = this->scene->cached_point_lights[0]->color;

					float dot = vec3_vec3_dot_val(intersection.normal, vec3_normalize_val(vec3_vec3_sub_val(this->scene->cached_point_lights[0]->position, intersection.position(&intersection))));
					result_color = (t_vec3){{light_color.x*dot*1.0f/dist + this->scene->ambient_color.x - intersection.shape->absorb_color.x, light_color.y*dot*1.0f/dist + this->scene->ambient_color.y  - intersection.shape->absorb_color.y, light_color.z*dot*1.0f/dist  + this->scene->ambient_color.z - intersection.shape->absorb_color.z}};
//					printf("Dot: %f\n", dot);
//					result_color = (t_vec3){{light_color.x, light_color.y, light_color.z}};
//					result_color = (t_vec3){{0, 0, 0}};
				}
                else
					result_color = (t_vec3){{this->scene->ambient_color.x - intersection.shape->absorb_color.x, this->scene->ambient_color.y - intersection.shape->absorb_color.y, this->scene->ambient_color.z - intersection.shape->absorb_color.z}};
//					result_color = (t_vec3){{0, 0, 0}};
				vec3_clamp_ptr(&result_color, 0.0f, 1.0f);
            	this->framebuffer->set_pixel(this->framebuffer, (t_ivec2){{x, y}}, result_color);
            }
            else
				this->framebuffer->set_pixel(this->framebuffer, (t_ivec2){{x, y}}, this->scene->ambient_color);
        }
    }
}

static void postrender(t_mainloop *this)
{
    this->framebuffer->unlock(this->framebuffer);
    this->framebuffer->put(this->framebuffer);
}

void input(t_mainloop *this)
{
    static float x_angle = 0.0f;
    static float y_angle = 0.0f;

    this->input_manager->update(this->input_manager);
    if(this->input_manager->is_close_event_pending ||
            this->input_manager->is_key_down(this->input_manager, SDLK_ESCAPE))
        this->stop(this);
    if(this->input_manager->is_key_down(this->input_manager, SDLK_UP))
        x_angle -= this->deltatime*1.0f;
    if(this->input_manager->is_key_down(this->input_manager, SDLK_DOWN))
        x_angle += this->deltatime*1.0f;
    if(this->input_manager->is_key_down(this->input_manager, SDLK_LEFT))
        y_angle += this->deltatime*1.0f;
    if(this->input_manager->is_key_down(this->input_manager, SDLK_RIGHT))
        y_angle -= this->deltatime*1.0f;
    t_vec3 up;
    up.x = 100.0f * sinf(x_angle) * cosf(y_angle);
    up.y = 100.0f * cosf(x_angle);
    up.z = 100.0f * sinf(x_angle) * sinf(y_angle);
    this->camera->look_at(this->camera, (t_vec3){{-5.0f, 1.0f, 0.0f}}, up, (t_vec3){{0.0f, -1.0f, 0.0f}});
//    if(this->input_manager->mouse_delta.x > 0 || this->input_manager->mouse_delta.y > 0)
//    {
//        x_angle += -(float)this->input_manager->mouse_delta.y/500.0f;
//        y_angle += -(float)this->input_manager->mouse_delta.x/500.0f;
//        t_vec3 up;
//        up.x = 10.0f * sinf(x_angle) * cosf(y_angle);
//        up.y = 10.0f * cosf(x_angle);
//        up.z = 10.0f * sinf(x_angle) * sinf(y_angle);
//        this->camera->look_at(this->camera, (t_vec3){{-5.0f, 1.0f, 0.0f}}, up, (t_vec3){{0.0f, -1.0f, 0.0f}});
//    }
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
	this->time_since_start = 1.0f;
    this->frame_count = 0;
    this->max_fps = 60;

    this->check_timer = construct_check_timer();
    this->sdl_instance = construct_sdl_instance(resolution, title);
    this->input_manager = construct_input_manager(this->sdl_instance);
    this->framebuffer = construct_framebuffer(resolution, this->sdl_instance);
    this->camera = construct_camera(M_PI/4.0f, (float)this->framebuffer->resolution.x/this->framebuffer->resolution.y);
    this->camera->look_at(this->camera, (t_vec3){{-5.0f, 1.0f, 0.0f}}, (t_vec3){{0.0f, 0.0f, 0.0f}}, (t_vec3){{0.0f, -1.0f, 0.0f}});
    this->render_mask = create_render_mask(resolution.x * resolution.y, 90);
    this->scene = construct_scene();
    this->scene->ambient_color = (t_vec3){{0.3f, 0.3f, 0.3f}};

	t_shape *shape;
    shape = construct_shape_plane((t_vec3){{0.0f, 0.0f, 0.0f}}, (t_vec3){{0.0f, 1.0f, 0.0f}}, "plane1");
    shape->absorb_color = (t_vec3){{1.0f - 1.0f, 1.0f - 0.0f, 1.0f - 0.0f}};
    this->scene->add_shape(this->scene, &shape);
    shape = construct_shape_plane((t_vec3){{0.0f, 6.0f, 0.0f}}, (t_vec3){{0.0f, -1.0f, 0.0f}}, "plane1");
    shape->absorb_color = (t_vec3){{1.0f - 0.0f, 1.0f - 1.0f, 1.0f - 1.0f}};
    this->scene->add_shape(this->scene, &shape);
    shape = construct_shape_plane((t_vec3){{0.0f, 0.0f, 5.0f}}, (t_vec3){{0.0f, 0.0f, -1.0f}}, "plane1");
    shape->absorb_color = (t_vec3){{1.0f - 0.0f, 1.0f - 0.0f, 1.0f - 1.0f}};
    this->scene->add_shape(this->scene, &shape);
    shape = construct_shape_plane((t_vec3){{0.0f, 0.0f, -5.0f}}, (t_vec3){{0.0f, 0.0f, 1.0f}}, "plane1");
    shape->absorb_color = (t_vec3){{1.0f - 1.0f, 1.0f - 0.0f, 1.0f - 1.0f}};
    this->scene->add_shape(this->scene, &shape);
    shape = construct_shape_plane((t_vec3){{5.0f, 0.0f, 0.0f}}, (t_vec3){{-1.0f, 0.0f, 0.0f}}, "plane1");
    shape->absorb_color = (t_vec3){{1.0f - 0.0f, 1.0f - 1.0f, 1.0f - 0.0f}};
    this->scene->add_shape(this->scene, &shape);
	shape = construct_shape_sphere((t_vec3){{0.0f, 1.0f, 0.0f}}, 1.0f, "sphere1");
	shape->absorb_color = (t_vec3){{1.0f - 1.0f, 1.0f - 0.0f, 1.0f - 1.0f}};
	this->scene->add_shape(this->scene, &shape);

//	shape = construct_shape_sphere((t_vec3){{0.0f, 5.0f, 0.0f}}, 1.0f, "sphere1");
//	shape->absorb_color = (t_vec3){{0.0f, 1.0f, 1.0f}};
//	this->scene->add_shape(this->scene, &shape);
//	shape = construct_shape_plane((t_vec3){{0.0f, 0.0f, 0.0f}}, (t_vec3){{-1.0f, 0.0f, 0.0f}}, "plane1");
//	shape->absorb_color = (t_vec3){{0.0f, 0.0f, 1.0f}};
//	this->scene->add_shape(this->scene, &shape);

	this->scene->add_point_light(this->scene, (t_vec3){{0.0f, 5.0f, 0.0f}}, (t_vec3){{3.0f, 3.0f, 3.0f}}, "My first point light");

    this->scene->list(this->scene);
    this->scene->cache(this->scene);
    return (this);
}
