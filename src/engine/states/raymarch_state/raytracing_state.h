#ifndef RAYTRACER_RAYTRACING_STATE_H
#define RAYTRACER_RAYTRACING_STATE_H

#include "../../state.h"
#include "../../input_manager.h"
#include "../../framebuffer.h"
#include "camera.h"
#include "scene.h"
#include "../../mainloop.h"

typedef struct	s_raytracing_state
{
//	t_camera *camera;
//	t_scene *scene;
	t_input_manager *input_manager;
	t_mainloop *mainloop;
	t_sdl_instance *sdl_instance;
	t_framebuffer *framebuffer;

	cl_int *random_lookup;
	size_t random_lookup_size;
	cl_int random_number;
	cl_int skip_percentage;

	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel main_kernel;
	cl_device_id device_id;
	cl_mem mem_array_obj;
}				t_raytracing_state;

t_state		*construct_raytracing_state(t_input_manager *input_manager, t_sdl_instance *sdl_instance, t_mainloop *mainloop);

#endif //RAYTRACER_RAYTRACING_STATE_H
