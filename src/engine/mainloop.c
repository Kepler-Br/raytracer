#include "mainloop.h"
#include "../states/raymarch_state/shape.h"
#include "ray.h"

static void run(t_mainloop *this)
{
    float lag;

	SDL_assert(this->state_stack != NULL);
    lag = 0.01f;
    this->is_running = 1;
    this->check_timer->check(this->check_timer);
    while(this->is_running)
    {
        this->calc_deltatime(this);
		lag += this->deltatime;
		this->time_since_start += this->deltatime;

		this->top_state->pre_render(this->top_state);
		this->top_state->render(this->top_state);
		this->top_state->post_render(this->top_state);
		this->input_manager->update(this->input_manager);
		this->top_state->input(this->top_state);
		if(this->is_running == 0)
			break;
		this->top_state->update(this->top_state, this->deltatime);
		this->top_state->late_update(this->top_state);
        while(lag > this->fixed_update_step_time)
        {

			this->top_state->fixed_update(this->top_state, this->fixed_update_step_time);
            lag -= this->fixed_update_step_time;
        }
        this->sdl_instance->present(this->sdl_instance);
        this->frame_count++;
        if(this->frame_count % ((int)(1.0f/this->deltatime) == 0 ? 1 : (int)(1.0f/this->deltatime)) == 0)
            printf("%d\n", (int)(1.0f/this->deltatime));
        this->limit_fps(this);
        this->input_manager->mouse_delta.x = 0.0f;
        this->input_manager->mouse_delta.y = 0.0f;
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

void push_state(struct s_mainloop *this, t_state *state)
{
	SDL_assert(state != NULL);
	double_ll_push_front((void *)&state, sizeof(t_state **), &this->state_stack, NULL);
	this->top_state = *(t_state **)double_ll_front(this->state_stack)->content;
}

void pop_state(struct s_mainloop *this)
{
	t_state *state_to_pop;

	SDL_assert(double_ll_len(this->state_stack) != 0);
	state_to_pop = *(t_state **)double_ll_front(this->state_stack)->content;
	state_to_pop->destructor(state_to_pop);
	double_ll_pop_front(&this->state_stack, NULL);
	free(state_to_pop);
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
    this->stop = &stop;
    this->limit_fps = &limit_fps;
    this->push_state = &push_state;
    this->pop_state = &pop_state;

    this->deltatime = 1.0f;
	this->time_since_start = 1.0f;
    this->frame_count = 0;
    this->max_fps = 60;
    this->fixed_update_step_time = 0.01f;
	this->state_stack = NULL;

    this->check_timer = construct_check_timer();
    this->sdl_instance = construct_sdl_instance(resolution, title);
    this->input_manager = construct_input_manager(this->sdl_instance);
    return (this);
}

void destruct_mainloop(t_mainloop *this)
{
	this->pop_state(this);
	free(this->check_timer);
	destruct_sdl_instance(this->sdl_instance);
	free(this->sdl_instance);
	destruct_input_manager(this->input_manager);
	free(this->input_manager);
}