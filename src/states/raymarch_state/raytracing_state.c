#include "raytracing_state.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "../../ocl_wrapper/ocl_wrapper.h"

static void			pre_render(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
	state->framebuffer->lock(state->framebuffer);
}

static void			render(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
}

static void			post_render(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
	state->framebuffer->unlock(state->framebuffer);
	state->framebuffer->put(state->framebuffer);
}

static void			update(struct s_state *this, float deltatime)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
}

static void			late_update(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
}

static void			fixed_update(struct s_state *this, float deltatime)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
}

static void 			input(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
	if(state->input_manager->is_close_event_pending)
		state->mainloop->stop(state->mainloop);
	if(state->input_manager->is_key_down(state->input_manager, SDLK_ESCAPE))
		state->mainloop->stop(state->mainloop);
}

static void 			on_stop(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
}

static void			destructor(struct s_state *this)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
	free(state->random_lookup);
	destruct_framebuffer(state->framebuffer);
	free(state->framebuffer);
	//		clReleaseMemObject(mem_array_obj);
	clReleaseKernel(state->main_kernel);
	clReleaseProgram(state->program);
	clReleaseDevice(state->device_id);
	clReleaseCommandQueue(state->commands);
	clReleaseContext(state->context);
	free(state);
}

t_state		*construct_raytracing_state(t_input_manager *input_manager, t_sdl_instance *sdl_instance, t_mainloop *mainloop)
{
	t_state				*state;
	t_raytracing_state	*raytracing_state;
	const size_t lookup_random_size = 0xFFFF;
	size_t index;

	SDL_assert((state = malloc(sizeof(t_state))) != NULL);
	SDL_assert((raytracing_state = malloc(sizeof(t_raytracing_state))) != NULL);
	state->instance_struct = (void *)raytracing_state;
	raytracing_state->framebuffer = construct_framebuffer(sdl_instance->resolution, sdl_instance);

	raytracing_state->sdl_instance = sdl_instance;
	raytracing_state->input_manager = input_manager;
	raytracing_state->mainloop = mainloop;
//	raytracing_state->camera
//	raytracing_state->framebuffer
	//raytracing_state->random_lookup
	//raytracing_state->scene


	raytracing_state->random_lookup_size = lookup_random_size;
	SDL_assert((raytracing_state->random_lookup = malloc(sizeof(cl_int) * lookup_random_size)) != NULL);
	index = 0;
	while(index < lookup_random_size)
	{
		raytracing_state->random_lookup[index] = rand();
		index++;
	}

	raytracing_state->random_number = rand()%raytracing_state->random_lookup_size;
	raytracing_state->skip_percentage = 80;


	raytracing_state->device_id = ocl_get_device(OCL_WRAPPER_MAX_COMPUTE_UNITS);
	ocl_print_device_info_full(raytracing_state->device_id);
	raytracing_state->context = ocl_create_context(raytracing_state->device_id);
	raytracing_state->commands = ocl_create_command_queue(raytracing_state->context, raytracing_state->device_id);
	raytracing_state->program = ocl_load_and_build_program(raytracing_state->context, raytracing_state->device_id, "./OpenCLPrograms/helloworld.cl");
	raytracing_state->main_kernel = ocl_get_kernel(raytracing_state->program, "main_kernel");

	state->pre_render = &pre_render;
	state->render = &render;
	state->post_render = &post_render;
	state->update = &update;
	state->late_update = &late_update;
	state->fixed_update = &fixed_update;
	state->input = &input;
	state->on_stop = &on_stop;
	state->destructor = &destructor;
	return (state);
}
