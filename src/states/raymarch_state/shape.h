#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"

#define SHAPE_NONE 0
#define SHAPE_PLANE 1
#define SHAPE_SPHERE 2
#define SHAPE_POINT_LIGHT 3

typedef struct s_intersection t_intersection;
typedef struct s_ray t_ray;

typedef struct s_material
{
    t_vec3 color;
    cl_bool is_emissive;
} t_material;

typedef struct s_named_material
{
    char *name;
    t_material *material;
} t_named_material;

t_named_material *construct_named_material(char *name, t_material input_material);
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

    void (*destructor)(void *this);
} t_shape;

void destruct_shape(t_shape *this);

typedef struct s_point_light
{
    t_vec3 position;
    t_vec3 color;
    cl_float power;
} t_point_light;

t_shape *construct_shape_point_light(t_vec3 position, t_vec3 color, char *name);

typedef struct s_shape_plane
{
    t_vec3 position;
    t_vec3 normal;
    cl_uint material_index;
} t_shape_plane;

t_shape *construct_shape_plane(t_vec3 position, t_vec3 normal, char *name);

typedef struct s_shape_sphere
{
    t_vec3 position;
    cl_float radius;
    cl_uint material_index;
} t_shape_sphere;

t_shape *construct_shape_sphere(t_vec3 position, float radius, char *name);

#endif // SHAPE_H
