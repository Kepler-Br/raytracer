#ifndef RAYTRACER_RAYTRACING_STATE_H
#define RAYTRACER_RAYTRACING_STATE_H

#include "../../engine/state.h"
#include "../../engine/input_manager.h"
#include "../../engine/framebuffer.h"
#include "camera.h"
#include "scene/scene.h"
#include "../../engine/mainloop.h"
#include "scene/cache.h"

typedef struct s_render_options
{
    cl_float exposure;
    cl_float gamma;
    cl_float contrast;
    cl_float brightness;
    cl_float lerp_coefficient;
    cl_uint spp;
    cl_bool only_direct;
    cl_bool only_indirect;
} t_render_options;

typedef struct	s_raytracing_state
{
	t_camera *camera;
	t_scene_items *scene_items;
	t_cache *cache;
	t_input_manager *input_manager;
	t_mainloop *mainloop;
	t_sdl_instance *sdl_instance;
	t_framebuffer *framebuffer;

	cl_int *random_lookup;
	size_t *global_work_size;
	size_t *local_work_size;
	cl_int random_lookup_size;
	cl_int random_number;
	cl_int skip_percentage;

	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel main_kernel;
	cl_device_id device_id;

	cl_mem mem_image;
    cl_mem mem_random_lookup;

	cl_mem mem_plane_list;
	cl_mem mem_sphere_list;
	cl_mem mem_aabb_list;
	cl_mem mem_triangle_list;
	cl_mem mem_square_list;
	cl_mem mem_disk_list;
	cl_mem mem_cylinder_list;
	cl_mem mem_cone_list;
    cl_mem mem_point_light_list;
    cl_mem mem_material_list;
    cl_mem mem_shape_list;

    t_vec2 camera_look_angle;
    t_vec3 camera_position;
}				t_raytracing_state;

t_state		*construct_raytracing_state(t_input_manager *input_manager, t_sdl_instance *sdl_instance, t_mainloop *mainloop);

#endif //RAYTRACER_RAYTRACING_STATE_H
