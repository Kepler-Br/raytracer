//
// Created by Nage Sena on 9/16/20.
//

#ifndef RAYTRACER_CACHE_H
#define RAYTRACER_CACHE_H

#include "OpenCL/cl.h"
#include "../shape.h"

typedef struct s_scene_items t_scene_items;

typedef struct s_cache
{
	t_shape_plane *cached_planes;
	t_shape_sphere *cached_spheres;
	t_shape_aabb *cached_aabb;
	t_shape_triangle *cached_triangles;
	t_shape_square *cached_squares;
	t_shape_disk *cached_disks;
	t_shape_cylinder *cached_cylinders;
	t_shape_cone *cached_cones;
	t_opencl_shape *cached_shapes;
	t_material *cached_materials;
	t_point_light *cached_point_lights;

	cl_int plane_cache_size;
	cl_int sphere_cache_size;
	cl_int aabb_cache_size;
	cl_int triangle_cache_size;
	cl_int square_cache_size;
	cl_int disk_cache_size;
	cl_int cylinder_cache_size;
	cl_int cone_cache_size;
	cl_int shape_cache_size;
	cl_int material_cache_size;
	cl_int point_light_cache_size;

	t_scene_items *scene;

	void (*cache_planes)(struct s_cache *this);
	void (*cache_spheres)(struct s_cache *this);
	void (*cache_aabbs)(struct s_cache *this);
	void (*cache_triangles)(struct s_cache *this);
	void (*cache_squares)(struct s_cache *this);
	void (*cache_disks)(struct s_cache *this);
	void (*cache_cylinders)(struct s_cache *this);
	void (*cache_cones)(struct s_cache *this);
	void (*cache_point_lights)(struct s_cache *this);
	void (*cache_materials)(struct s_cache *this);
	void (*cache_shapes)(struct s_cache *this);
	void (*cache)(struct s_cache *this);
} t_cache;

t_cache *construct_cache(t_scene_items *scene);

#endif //RAYTRACER_CACHE_H
