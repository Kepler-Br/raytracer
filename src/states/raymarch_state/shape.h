#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"

#define SHAPE_NONE 0
#define SHAPE_PLANE 1
#define SHAPE_SPHERE 2
#define SHAPE_AABB 3
#define SHAPE_TRIANGLE 4
#define SHAPE_SQUARE 5
#define SHAPE_DISK 6
#define SHAPE_CYLINDER 7
#define SHAPE_CONE 8
#define SHAPE_POINT_LIGHT 9

typedef struct s_intersection t_intersection;
typedef struct s_ray t_ray;

typedef struct s_material
{
    t_vec3 albedo;
    cl_float metallic;
    cl_float specular;
    cl_float roughess;
    cl_float anisotropic;
    cl_bool is_emissive;
} t_material;

typedef struct s_named_material
{
    char *name;
    t_material *material;
} t_named_material;

t_named_material *construct_named_material(t_material *material, char *name);
void destruct_named_material(t_named_material *material);

typedef struct  s_opencl_shape
{
    cl_int shape_type;
    cl_int shape_index;
}               t_opencl_shape;

typedef struct s_shape
{
    void *inhereted;
    char *name;
    int shape_type;
} t_shape;

t_shape *construct_shape(void *shape, int shape_type, char *name);
void destruct_shape(t_shape *shape);

typedef struct s_point_light
{
    t_vec3 position;
    t_vec3 color;
    cl_float power;
    cl_float radius;
} t_point_light;

typedef struct s_shape_plane
{
    t_vec3 position;
    t_vec3 normal;
    cl_uint material_index;
} t_shape_plane;

typedef struct s_shape_sphere
{
    t_vec3 position;
    cl_float radius;
    cl_uint material_index;
} t_shape_sphere;

typedef struct s_shape_aabb
{
	t_vec3 min;
	t_vec3 max;
	t_vec3 position;
	cl_uint material_index;
} t_shape_aabb;

typedef struct s_shape_triangle
{
	t_vec3 point1;
	t_vec3 point2;
	t_vec3 point3;
	t_vec3 position;
	cl_uint material_index;
} t_shape_triangle;

typedef struct s_shape_square
{
	t_vec3 point1;
	t_vec3 point2;
	t_vec3 point3;
	t_vec3 point4;
	t_vec3 position;
	cl_uint material_index;
} t_shape_square;

typedef struct s_shape_disk
{
	cl_float radius;
	t_vec3 position;
	cl_uint material_index;
} t_shape_disk;

typedef struct s_shape_cylinder
{
	cl_uint material_index;
} t_shape_cylinder;

typedef struct s_shape_cone
{
	cl_uint material_index;
} t_shape_cone;

#endif // SHAPE_H
