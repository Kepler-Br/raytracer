#include "raytracing_state.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "../../ocl_wrapper/ocl_wrapper.h"

static void			pre_render(struct s_state *this)
{
	t_raytracing_state *state;
	cl_int2 random_number;

    random_number.x = random();
    random_number.y = random();

	state = (t_raytracing_state *)this->instance_struct;
	state->framebuffer->lock(state->framebuffer);

//	ocl_enqueue_write_buffer(state->commands, state->mem_sphere_list, sizeof(t_shape_sphere)*state->scene_items->sphere_cache_size, (void*)state->scene_items->cached_spheres);
//	ocl_enqueue_write_buffer(state->commands, state->mem_point_light_list, sizeof(t_shape_sphere)*state->scene_items->point_light_cache_size, (void*)state->scene_items->cached_point_lights);
	t_cache *cache = state->cache;
    ocl_set_kernel_arg(state->main_kernel, 0, sizeof(cl_mem), (void *)&state->mem_image);
    ocl_set_kernel_arg(state->main_kernel, 1, sizeof(cl_mem), (void *)&state->mem_random_lookup);
    ocl_set_kernel_arg(state->main_kernel, 2, sizeof(cl_int), (void *)&state->random_lookup_size);
    ocl_set_kernel_arg(state->main_kernel, 3, sizeof(cl_int2), (void *)&state->framebuffer->resolution);
    ocl_set_kernel_arg(state->main_kernel, 4, sizeof(cl_int2), (void *)&random_number);
    ocl_set_kernel_arg(state->main_kernel, 5, sizeof(cl_int), (void *)&state->skip_percentage);
    ocl_set_kernel_arg(state->main_kernel, 6, sizeof(t_camera), (void *)state->camera);

	ocl_set_kernel_arg(state->main_kernel, 7, sizeof(cl_mem), (void *)&state->mem_plane_list);
	ocl_set_kernel_arg(state->main_kernel, 8, sizeof(cl_int), (void *)&cache->plane_cache_size);

    ocl_set_kernel_arg(state->main_kernel, 9, sizeof(cl_mem), (void *)&state->mem_sphere_list);
    ocl_set_kernel_arg(state->main_kernel, 10, sizeof(cl_int), (void *)&cache->sphere_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 11, sizeof(cl_mem), (void *)&state->mem_aabb_list);
	ocl_set_kernel_arg(state->main_kernel, 12, sizeof(cl_int), (void *)&cache->aabb_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 13, sizeof(cl_mem), (void *)&state->mem_triangle_list);
	ocl_set_kernel_arg(state->main_kernel, 14, sizeof(cl_int), (void *)&cache->triangle_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 15, sizeof(cl_mem), (void *)&state->mem_square_list);
	ocl_set_kernel_arg(state->main_kernel, 16, sizeof(cl_int), (void *)&cache->square_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 17, sizeof(cl_mem), (void *)&state->mem_disk_list);
	ocl_set_kernel_arg(state->main_kernel, 18, sizeof(cl_int), (void *)&cache->disk_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 19, sizeof(cl_mem), (void *)&state->mem_cylinder_list);
	ocl_set_kernel_arg(state->main_kernel, 20, sizeof(cl_int), (void *)&cache->cylinder_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 21, sizeof(cl_mem), (void *)&state->mem_cone_list);
	ocl_set_kernel_arg(state->main_kernel, 22, sizeof(cl_int), (void *)&cache->cone_cache_size);

	ocl_set_kernel_arg(state->main_kernel, 23, sizeof(cl_mem), (void *)&state->mem_point_light_list);
    ocl_set_kernel_arg(state->main_kernel, 24, sizeof(cl_int), (void *)&cache->point_light_cache_size);

    ocl_set_kernel_arg(state->main_kernel, 25, sizeof(cl_mem), (void *)&state->mem_material_list);
    ocl_set_kernel_arg(state->main_kernel, 26, sizeof(cl_int), (void *)&cache->material_cache_size);

    ocl_set_kernel_arg(state->main_kernel, 27, sizeof(cl_mem), (void *)&state->mem_shape_list);
	ocl_set_kernel_arg(state->main_kernel, 28, sizeof(cl_int), (void *)&cache->shape_cache_size);

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
	state->camera_look_angle.x -= (float)state->input_manager->mouse_delta.x/3*deltatime;
	state->camera_look_angle.y += (float)state->input_manager->mouse_delta.y/3*deltatime;
	if (state->camera_look_angle.y > M_PI*0.999f)
		state->camera_look_angle.y = (float)M_PI*0.999f;
    if (state->camera_look_angle.y < 0.001f)
        state->camera_look_angle.y = 0.001f;

	if (state->camera_look_angle.x > M_PI*2)
		state->camera_look_angle.x -= M_PI*2;
	if (state->camera_look_angle.x < -M_PI*2)
		state->camera_look_angle.x += M_PI*2;
//    camera_look_at(state->camera, (t_vec3) {{5.0f, 5.0f, 5.0f}},
//                   (t_vec3) {{0.0f, 0.0f, 0.0f}},
//                   (t_vec3) {{0.0f, -1.0f, 0.0f}});


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


//	((t_shape_sphere *)((t_shape *)state->scene_items->spheres->content)->inhereted)->position.x += sin(state->mainloop->time_since_start)*100;
//	state->scene_items->cached_spheres[0].position.x += sinf(state->mainloop->time_since_start)*deltatime;
//	state->scene_items->cached_spheres[0].position.y += sinf(state->mainloop->time_since_start)*deltatime;
//	state->scene_items->cached_point_lights[0].position.x += sinf(state->mainloop->time_since_start)*deltatime*10.0f;
//	state->scene_items->cached_point_lights[0].position.y += cosf(state->mainloop->time_since_start)*deltatime;
	t_vec3 forward = (t_vec3){{cosf(state->camera_look_angle.x) * sinf(state->camera_look_angle.y),
								cosf(state->camera_look_angle.y),
                                sinf(state->camera_look_angle.x) * sinf(state->camera_look_angle.y)}};
	t_vec3 right = vec3_vec3_cross_val((t_vec3){{0.0f, 1.0f, 0.0f}}, forward);
	t_vec3 up = vec3_vec3_cross_val(right, forward);
    forward = vec3_vec3_sum_val(forward, state->camera_position);

	camera_look_at(state->camera, state->camera_position,
                   forward, up);
	cl_float speed = 1.0f;
	if(state->input_manager->is_key_down(state->input_manager, SDLK_LSHIFT))
		speed = 1.8f;
    if(state->input_manager->is_key_down(state->input_manager, SDLK_w))
        state->camera_position = vec3_vec3_sum_val(state->camera_position, vec3_scalar_div_val(state->camera->forward, 10.0f/speed));
    if(state->input_manager->is_key_down(state->input_manager, SDLK_s))
        state->camera_position = vec3_vec3_sum_val(state->camera_position, vec3_scalar_div_val(state->camera->forward, -10.0f/speed));
    if(state->input_manager->is_key_down(state->input_manager, SDLK_a))
        state->camera_position = vec3_vec3_sum_val(state->camera_position, vec3_scalar_div_val(state->camera->right, -10.0f/speed));
    if(state->input_manager->is_key_down(state->input_manager, SDLK_d))
        state->camera_position = vec3_vec3_sum_val(state->camera_position, vec3_scalar_div_val(state->camera->right, 10.0f/speed));
    if(state->input_manager->is_key_down(state->input_manager, SDLK_SPACE))
        state->camera_position = vec3_vec3_sum_val(state->camera_position, vec3_scalar_div_val(state->camera->up, -10.0f/speed));
    if(state->input_manager->is_key_down(state->input_manager, SDLK_LCTRL))
        state->camera_position = vec3_vec3_sum_val(state->camera_position, vec3_scalar_div_val(state->camera->up, 10.0f/speed));
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

	clReleaseMemObject(state->mem_plane_list);
	clReleaseMemObject(state->mem_sphere_list);
	clReleaseMemObject(state->mem_aabb_list);
	clReleaseMemObject(state->mem_triangle_list);
	clReleaseMemObject(state->mem_square_list);
	clReleaseMemObject(state->mem_disk_list);
	clReleaseMemObject(state->mem_cylinder_list);
	clReleaseMemObject(state->mem_cone_list);
	clReleaseMemObject(state->mem_point_light_list);
	clReleaseMemObject(state->mem_material_list);
	clReleaseMemObject(state->mem_shape_list);
	clReleaseKernel(state->main_kernel);
	clReleaseProgram(state->program);
	clReleaseDevice(state->device_id);
	clReleaseCommandQueue(state->commands);
	clReleaseContext(state->context);
	free(state);
}


static void temp_add_materials(t_raytracing_state *raytracing_state)
{
	t_material *material;

    material = malloc(sizeof(t_material));
    material->albedo = (t_vec3){{0.0f, 1.0f, 0.0f}};
    material->anisotropic = 0.0f;
    material->metallic = 0.0f;
    material->roughess = 0.0f;
    material->specular = 0.0f;
    material->is_emissive = CL_FALSE;
    raytracing_state->scene_items->add_material(raytracing_state->scene_items, material, "green");

	material = malloc(sizeof(t_material));
	material->albedo = (t_vec3){{1.0f, 0.0f, 1.0f}};
	material->anisotropic = 0.0f;
	material->metallic = 0.0f;
	material->roughess = 0.0f;
	material->specular = 0.0f;
	material->is_emissive = CL_FALSE;
	raytracing_state->scene_items->add_material(raytracing_state->scene_items, material, "purple");

	material = malloc(sizeof(t_material));
	material->albedo = (t_vec3){{1.0f, 0.0f, 0.0f}};
	material->anisotropic = 0.0f;
	material->metallic = 0.0f;
	material->roughess = 0.0f;
	material->specular = 0.0f;
	material->is_emissive = CL_FALSE;
	raytracing_state->scene_items->add_material(raytracing_state->scene_items, material, "red");

	material = malloc(sizeof(t_material));
	material->albedo = (t_vec3){{1.0f, 1.0f, 1.0f}};
//    material->albedo = vec3_scalar_mul(&material->albedo, 1000000.0f);
	material->anisotropic = 0.0f;
	material->metallic = 0.0f;
	material->roughess = 0.0f;
	material->specular = 0.0f;
	material->is_emissive = CL_TRUE;
	material->emission_power = 1000.0f;
	raytracing_state->scene_items->add_material(raytracing_state->scene_items, material, "emissive");
}

static void temp_add_shapes(t_raytracing_state *raytracing_state)
{
	t_shape_sphere *sphere;
	t_shape_plane *plane;
	t_point_light *point_light;

    sphere = malloc(sizeof(t_shape_sphere));
    sphere->material_index = raytracing_state->scene_items->get_material_index(raytracing_state->scene_items, "green");
    sphere->position = (t_vec3){{4.0f, 1.0f, 2.0f}};
    sphere->radius = 1.0f;
    raytracing_state->scene_items->add_shape(raytracing_state->scene_items, sphere, SHAPE_SPHERE, "near_center_sphere");

	sphere = malloc(sizeof(t_shape_sphere));
	sphere->material_index = raytracing_state->scene_items->get_material_index(raytracing_state->scene_items, "purple");
	sphere->position = (t_vec3){{0.0f, 2.0f, 0.0f}};
	sphere->radius = 2.0f;
	raytracing_state->scene_items->add_shape(raytracing_state->scene_items, sphere, SHAPE_SPHERE, "center_sphere");

	sphere = malloc(sizeof(t_shape_sphere));
	sphere->material_index = raytracing_state->scene_items->get_material_index(raytracing_state->scene_items, "emissive");
	sphere->position = (t_vec3){{0.0f, 6.0f, 0.0f}};
	sphere->radius = 1.0f;
	raytracing_state->scene_items->add_shape(raytracing_state->scene_items, sphere, SHAPE_SPHERE, "floor_sphere");

	plane = malloc(sizeof(t_shape_plane));
	plane->material_index = raytracing_state->scene_items->get_material_index(raytracing_state->scene_items, "red");
	plane->position = (t_vec3){{0.0f, 0.0f, 0.0f}};
	plane->normal = (t_vec3){{0.0f, 1.0f, 0.0f}};
	raytracing_state->scene_items->add_shape(raytracing_state->scene_items, plane, SHAPE_PLANE, "floor_plane");

    point_light = malloc(sizeof(t_point_light));
    point_light->color = (t_vec3){{1.0f, 1.0f, 1.0f}};
    point_light->position = (t_vec3){{5.0f, 6.0f, 0.0f}};
    point_light->power = 0.0f;
    raytracing_state->scene_items->add_shape(raytracing_state->scene_items, point_light, SHAPE_POINT_LIGHT, "main_point_light");
}

t_state		*construct_raytracing_state(t_input_manager *input_manager, t_sdl_instance *sdl_instance, t_mainloop *mainloop)
{
	t_state				*state;
	t_raytracing_state	*raytracing_state;
	size_t index;

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_assert((state = malloc(sizeof(t_state))) != NULL);
	SDL_assert((raytracing_state = malloc(sizeof(t_raytracing_state))) != NULL);
	state->instance_struct = (void *)raytracing_state;

	raytracing_state->framebuffer = construct_framebuffer(ivec2_scalar_div(&sdl_instance->resolution, 2), sdl_instance);
	raytracing_state->sdl_instance = sdl_instance;
	raytracing_state->input_manager = input_manager;
	raytracing_state->mainloop = mainloop;

    raytracing_state->camera_look_angle = (t_vec2){{0.0f, 0.0f}};
    raytracing_state->camera_position = (t_vec3){{5.0f, 5.0f, 5.0}};
    SDL_assert((raytracing_state->global_work_size = malloc(sizeof(size_t)*2)) != NULL);
    SDL_assert((raytracing_state->local_work_size = malloc(sizeof(size_t)*2)) != NULL);
    raytracing_state->camera = construct_camera(M_PI_4,
          (float)raytracing_state->framebuffer->resolution.x/raytracing_state->framebuffer->resolution.y);
    camera_look_at(raytracing_state->camera, raytracing_state->camera_position,
                   (t_vec3) {{0.0f, 0.0f, 0.0f}},
                   (t_vec3) {{0.0f, -1.0f, 0.0f}});
    raytracing_state->global_work_size[0] = raytracing_state->framebuffer->resolution.x;
    raytracing_state->global_work_size[1] = raytracing_state->framebuffer->resolution.y;
    raytracing_state->local_work_size[0] = 10;
    raytracing_state->local_work_size[1] = 10;
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
    raytracing_state->cache = construct_cache(raytracing_state->scene_items);

	temp_add_materials(raytracing_state);
	temp_add_shapes(raytracing_state);

	raytracing_state->scene_items->list(raytracing_state->scene_items);
	raytracing_state->cache->cache(raytracing_state->cache);

	raytracing_state->device_id = ocl_get_device(OCL_WRAPPER_MAX_COMPUTE_UNITS);
	ocl_print_device_info_full(raytracing_state->device_id);
	raytracing_state->context = ocl_create_context(raytracing_state->device_id);
	raytracing_state->commands = ocl_create_command_queue(raytracing_state->context, raytracing_state->device_id);
	raytracing_state->program = ocl_load_and_build_program(raytracing_state->context, raytracing_state->device_id, "./OpenCLPrograms/main.cl");
	raytracing_state->main_kernel = ocl_get_kernel(raytracing_state->program, "main_kernel");

	t_cache *cache = raytracing_state->cache;

	size_t size;
	size = raytracing_state->framebuffer->pixel_count*raytracing_state->framebuffer->color_count*sizeof(cl_char);
	raytracing_state->mem_image = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_WRITE, size, NULL);

	size = lookup_random_size*sizeof(cl_int);
    raytracing_state->mem_random_lookup = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
    ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_random_lookup, lookup_random_size*sizeof(cl_int), raytracing_state->random_lookup);


    size = (size_t)cache->plane_cache_size*sizeof(t_shape_plane);
    size = size == 0 ? sizeof(t_shape_plane) : size;
    raytracing_state->mem_plane_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
    if(cache->plane_cache_size != 0)
    	ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_plane_list, size, cache->cached_planes);

	size = (size_t)cache->sphere_cache_size*sizeof(t_shape_sphere);
	size = size == 0 ? sizeof(t_shape_sphere) : size;
	raytracing_state->mem_sphere_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->sphere_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_sphere_list, size, cache->cached_spheres);

	size = (size_t)cache->aabb_cache_size*sizeof(t_shape_aabb);
	size = size == 0 ? sizeof(t_shape_aabb) : size;
	raytracing_state->mem_aabb_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->aabb_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_aabb_list, size, cache->cached_aabb);

	size = (size_t)cache->triangle_cache_size*sizeof(t_shape_triangle);
	size = size == 0 ? sizeof(t_shape_triangle) : size;
	raytracing_state->mem_triangle_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->triangle_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_triangle_list, size, cache->cached_triangles);

	size = (size_t)cache->square_cache_size*sizeof(t_shape_square);
	size = size == 0 ? sizeof(t_shape_square) : size;
	raytracing_state->mem_square_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->square_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_square_list, size, cache->cached_squares);

	size = (size_t)cache->disk_cache_size*sizeof(t_shape_disk);
	size = size == 0 ? sizeof(t_shape_disk) : size;
	raytracing_state->mem_disk_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->disk_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_disk_list, size, cache->cached_disks);

	size = (size_t)cache->cylinder_cache_size*sizeof(t_shape_cylinder);
	size = size == 0 ? sizeof(t_shape_cylinder) : size;
	raytracing_state->mem_cylinder_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->cylinder_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_cylinder_list, size, cache->cached_cylinders);

	size = (size_t)cache->cone_cache_size*sizeof(t_shape_cone);
	size = size == 0 ? sizeof(t_shape_cone) : size;
	raytracing_state->mem_cone_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->cone_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_cone_list, size, cache->cached_cones);

	size = (size_t)cache->point_light_cache_size*sizeof(t_point_light);
	size = size == 0 ? sizeof(t_point_light) : size;
	raytracing_state->mem_point_light_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->point_light_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_point_light_list, size, cache->cached_point_lights);

	size = (size_t)cache->material_cache_size*sizeof(t_material);
	size = size == 0 ? sizeof(t_material) : size;
	raytracing_state->mem_material_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->material_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_material_list, size, cache->cached_materials);

	size = (size_t)cache->shape_cache_size*sizeof(t_opencl_shape);
	size = size == 0 ? sizeof(t_opencl_shape) : size;
	raytracing_state->mem_shape_list = ocl_create_buffer(raytracing_state->context, CL_MEM_READ_ONLY, size, NULL);
	if(cache->shape_cache_size != 0)
		ocl_enqueue_write_buffer(raytracing_state->commands, raytracing_state->mem_shape_list, size, cache->cached_shapes);

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
