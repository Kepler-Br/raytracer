#include "raytracing_state.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "../../ocl_wrapper/ocl_wrapper.h"

static void			pre_render(struct s_state *this)
{
	t_raytracing_state *state;
	cl_int random_number;

	random_number = random();

	state = (t_raytracing_state *)this->instance_struct;
	state->framebuffer->lock(state->framebuffer);
//	t_shape_sphere *sphere;
//	sphere = malloc(sizeof(t_shape_sphere));
//	sphere->radius = 1.0f;
//	sphere->position = (t_vec3){{1.0f, 1.0f, 1.0f}};
//	sphere->material_index = 1;
//    ocl_enqueue_write_buffer(state->commands, state->mem_sphere_list, sizeof(t_shape_sphere), (void*)sphere);
//	ocl_enqueue_write_buffer(state->commands, state->mem_image, state->framebuffer->pixel_count*state->framebuffer->color_count, state->framebuffer->pixels);
    ocl_set_kernel_arg(state->main_kernel, 0, sizeof(cl_mem), (void *)&state->mem_image);
    ocl_set_kernel_arg(state->main_kernel, 1, sizeof(cl_mem), (void *)&state->mem_random_lookup);
    ocl_set_kernel_arg(state->main_kernel, 2, sizeof(cl_int), (void *)&state->random_lookup_size);
    ocl_set_kernel_arg(state->main_kernel, 3, sizeof(cl_int2), (void *)&state->framebuffer->resolution);
    ocl_set_kernel_arg(state->main_kernel, 4, sizeof(cl_int), (void *)&random_number);
    ocl_set_kernel_arg(state->main_kernel, 5, sizeof(cl_int), (void *)&state->skip_percentage);
    ocl_set_kernel_arg(state->main_kernel, 6, sizeof(t_camera), (void *)state->camera);
    ocl_set_kernel_arg(state->main_kernel, 7, sizeof(cl_mem), (void *)&state->mem_sphere_list);
    ocl_set_kernel_arg(state->main_kernel, 8, sizeof(cl_int), (void *)&state->scene_items->sphere_cache_size);
    ocl_set_kernel_arg(state->main_kernel, 9, sizeof(cl_mem), (void *)&state->mem_plane_list);
    ocl_set_kernel_arg(state->main_kernel, 10, sizeof(cl_int), (void *)&state->scene_items->plane_cache_size);
    ocl_set_kernel_arg(state->main_kernel, 11, sizeof(cl_mem), (void *)&state->mem_point_light_list);
    ocl_set_kernel_arg(state->main_kernel, 12, sizeof(cl_int), (void *)&state->scene_items->point_light_cache_size);
    ocl_set_kernel_arg(state->main_kernel, 13, sizeof(cl_mem), (void *)&state->mem_material_list);
    ocl_set_kernel_arg(state->main_kernel, 14, sizeof(cl_int), (void *)&state->scene_items->material_cache_size);
    ocl_set_kernel_arg(state->main_kernel, 15, sizeof(cl_mem), (void *)&state->mem_shape_list);
    ocl_set_kernel_arg(state->main_kernel, 16, sizeof(cl_int), (void *)&state->scene_items->shape_cache_size);
}

static void			render(struct s_state *this)
{
	t_raytracing_state *state;
    state = (t_raytracing_state *)this->instance_struct;

    ocl_enqueue_nd_range_kernel(state->commands, state->main_kernel, 2, state->global_work_size, state->local_work_size);
}

static void			post_render(struct s_state *this)
{
	t_raytracing_state *state;
	state = (t_raytracing_state *)this->instance_struct;

    ocl_finish(state->commands);
	ocl_enqueue_read_buffer(state->commands, state->mem_image, state->framebuffer->pixel_count*state->framebuffer->color_count, state->framebuffer->pixels);
	state->framebuffer->unlock(state->framebuffer);
	state->framebuffer->put(state->framebuffer);
}

static void			update(struct s_state *this, float deltatime)
{
	t_raytracing_state *state;

	state = (t_raytracing_state *)this->instance_struct;
    camera_look_at(state->camera, (t_vec3) {{5.0f, 5.0f, 5.0f}},
                   (t_vec3) {{0.0f, 0.0f, 0.0f}},
                   (t_vec3) {{0.0f, -1.0f, 0.0f}});
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
    free(state->global_work_size);
    free(state->local_work_size);
	destruct_framebuffer(state->framebuffer);
	free(state->framebuffer);
    clReleaseMemObject(state->mem_image);
    clReleaseMemObject(state->mem_random_lookup);
//    clReleaseMemObject(state->mem_random_lookup_size);
//    clReleaseMemObject(state->mem_screen_geometry);
//    clReleaseMemObject(state->mem_random_number);
//    clReleaseMemObject(state->mem_skip_percentage);
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

	size_t index;


	SDL_assert((state = malloc(sizeof(t_state))) != NULL);
	SDL_assert((raytracing_state = malloc(sizeof(t_raytracing_state))) != NULL);
	state->instance_struct = (void *)raytracing_state;

	raytracing_state->framebuffer = construct_framebuffer(sdl_instance->resolution, sdl_instance);
	raytracing_state->sdl_instance = sdl_instance;
	raytracing_state->input_manager = input_manager;
	raytracing_state->mainloop = mainloop;

    SDL_assert((raytracing_state->global_work_size = malloc(sizeof(size_t)*2)) != NULL);
    SDL_assert((raytracing_state->local_work_size = malloc(sizeof(size_t)*2)) != NULL);
    raytracing_state->camera = construct_camera(M_PI_4,
          (float)raytracing_state->framebuffer->resolution.x/raytracing_state->framebuffer->resolution.y);
    camera_look_at(raytracing_state->camera, (t_vec3) {{5.0f, 5.0f, 5.0f}},
                   (t_vec3) {{0.0f, 0.0f, 0.0f}},
                   (t_vec3) {{0.0f, -1.0f, 0.0f}});
    raytracing_state->global_work_size[0] = raytracing_state->framebuffer->resolution.x;
    raytracing_state->global_work_size[1] = raytracing_state->framebuffer->resolution.y;
    raytracing_state->local_work_size[0] = 16;
    raytracing_state->local_work_size[1] = 16;
    size_t lookup_random_size = raytracing_state->framebuffer->resolution.x * raytracing_state->framebuffer->resolution.y;
	raytracing_state->random_lookup_size = lookup_random_size;
	SDL_assert((raytracing_state->random_lookup = malloc(sizeof(cl_int) * lookup_random_size)) != NULL);
	index = 0;
	while(index < lookup_random_size)
	{
		raytracing_state->random_lookup[index] = random();
		index++;
	}
	raytracing_state->skip_percentage = 0;


    raytracing_state->scene_items = construct_scene_items();
    t_shape_sphere *sphere;
    sphere = malloc(sizeof(t_shape_sphere));
    sphere->material_index = 0;
    sphere->position = (t_vec3){{0.0f, 0.0f, 0.0f}};
    sphere->radius = 1.0f;
    raytracing_state->scene_items->add_sphere(raytracing_state->scene_items, sphere, "sphere1");
    sphere = malloc(sizeof(t_shape_sphere));
    sphere->position = (t_vec3){{0.0f, 3.0f, 0.0f}};
    sphere->radius = 1.0f;
    sphere->material_index = 1;
    raytracing_state->scene_items->add_sphere(raytracing_state->scene_items, sphere, "sphere2");
    t_shape_plane *plane;
    plane = malloc(sizeof(t_shape_plane));
    plane->normal = (t_vec3){{0.0f, -1.0f, 0.0f}};
    plane->position = (t_vec3){{0.0f, 0.0f, 0.0f}};
    plane->material_index = 1;
    raytracing_state->scene_items->add_plane(raytracing_state->scene_items, plane, "plane1");
    t_material *material;
    material = malloc(sizeof(t_material));
    material->absorb_color = (t_vec3){{0.0f, 1.0f, 0.0f}};
    material->emissive_color = (t_vec3){{0.0f, 0.0f, 0.0f}};
    material->is_emissive = CL_FALSE;
    raytracing_state->scene_items->add_material(raytracing_state->scene_items, material, "purple_material");
    material = malloc(sizeof(t_material));
    material->absorb_color = (t_vec3){{1.0f, 0.0f, 0.0f}};
    material->emissive_color = (t_vec3){{0.0f, 0.0f, 0.0f}};
    material->is_emissive = CL_FALSE;
    raytracing_state->scene_items->add_material(raytracing_state->scene_items, material, "red_material");
    t_point_light *point_light;
    point_light = malloc(sizeof(t_point_light));
    point_light->emission_color = (t_vec3){{1.0f, 1.0f, 1.0f}};
    point_light->position = (t_vec3){{5.0f, 5.0f, 5.0f}};
    raytracing_state->scene_items->add_point_light(raytracing_state->scene_items, point_light, "main_point_light");
    raytracing_state->scene_items->list(raytracing_state->scene_items);
    raytracing_state->scene_items->cache_full(raytracing_state->scene_items);

	raytracing_state->device_id = ocl_get_device(OCL_WRAPPER_MAX_COMPUTE_UNITS);
	ocl_print_device_info_full(raytracing_state->device_id);
	raytracing_state->context = ocl_create_context(raytracing_state->device_id);
	raytracing_state->commands = ocl_create_command_queue(raytracing_state->context, raytracing_state->device_id);
	raytracing_state->program = ocl_load_and_build_program(raytracing_state->context, raytracing_state->device_id, "./OpenCLPrograms/main.cl");
	raytracing_state->main_kernel = ocl_get_kernel(raytracing_state->program, "main_kernel");


	size_t size;
	size = raytracing_state->framebuffer->pixel_count*raytracing_state->framebuffer->color_count*sizeof(cl_char);
	raytracing_state->mem_image = ocl_create_buffer(raytracing_state->context, CL_MEM_WRITE_ONLY, size, NULL);
    raytracing_state->mem_random_lookup = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, lookup_random_size*sizeof(cl_int), NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_random_lookup, lookup_random_size*sizeof(cl_int), raytracing_state->random_lookup);
    raytracing_state->mem_sphere_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, (size_t)raytracing_state->scene_items->sphere_cache_size*sizeof(t_shape_sphere), NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_sphere_list, (size_t)raytracing_state->scene_items->sphere_cache_size*sizeof(t_shape_sphere), raytracing_state->scene_items->cached_spheres);
    raytracing_state->mem_plane_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, (size_t)raytracing_state->scene_items->plane_cache_size*sizeof(t_shape_plane), NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_plane_list, (size_t)raytracing_state->scene_items->plane_cache_size*sizeof(t_shape_plane), raytracing_state->scene_items->cached_planes);
    raytracing_state->mem_point_light_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, (size_t)raytracing_state->scene_items->point_light_cache_size*sizeof(t_point_light), NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_point_light_list, (size_t)raytracing_state->scene_items->point_light_cache_size*sizeof(t_point_light), raytracing_state->scene_items->cached_point_lights);
    raytracing_state->mem_material_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, (size_t)raytracing_state->scene_items->material_cache_size*sizeof(t_material), NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_material_list, (size_t)raytracing_state->scene_items->material_cache_size*sizeof(t_material), raytracing_state->scene_items->cached_materials);
    raytracing_state->mem_shape_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, (size_t)raytracing_state->scene_items->shape_cache_size*sizeof(t_opencl_shape), NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_shape_list, (size_t)raytracing_state->scene_items->shape_cache_size*sizeof(t_opencl_shape), raytracing_state->scene_items->cached_shapes);

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
