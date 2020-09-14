#ifndef SCENE_H
#define SCENE_H

#include "../shape.h"
#include "double_linked_list.h"

typedef struct s_scene_items
{
    t_shape_plane *cached_planes;
	t_shape_sphere *cached_spheres;
	t_shape_cube *cached_cubes;
    t_point_light *cached_point_lights;
    t_material *cached_materials;
    t_opencl_shape *cached_shapes;

    t_double_linked_list *planes;
    t_double_linked_list *spheres;
	t_double_linked_list *cubes;
    t_double_linked_list *point_lights;
    t_double_linked_list *materials;

    cl_int plane_cache_size;
	cl_int sphere_cache_size;
	cl_int cube_cache_size;
    cl_int point_light_cache_size;
    cl_int shape_cache_size;
    cl_int material_cache_size;

    void (*cache)(struct s_scene_items *this, int shape_type);
    void (*cache_full)(struct s_scene_items *this);
    void (*cache_materials)(struct s_scene_items *this);
    void (*add_sphere)(struct s_scene_items *this, t_shape_sphere *plane, char *name);
	void (*add_plane)(struct s_scene_items *this, t_shape_plane *sphere, char *name);
	void (*add_cube)(struct s_scene_items *this, t_shape_cube *cube, char *name);
    void (*destroy_shape)(struct s_scene_items *this, char *name);
    void (*destroy_similar_shapes)(struct s_scene_items *this, char *name);
    void (*update_cache)(struct s_scene_items *this, int shape_type);
    void (*add_point_light)(struct s_scene_items *this, t_point_light *point_light, char *name);
	void (*add_material)(struct s_scene_items *this, t_material *material, char *name);
	cl_uint (*material_index_from_name)(struct s_scene_items *this, char *name);
    void (*destroy_material)(struct s_scene_items *this, char *name);
	void (*list)(struct s_scene_items *this);
} t_scene_items;

t_scene_items *construct_scene_items();
void destruct_scene_items(struct s_scene_items *this);

#endif // SCENE_H
