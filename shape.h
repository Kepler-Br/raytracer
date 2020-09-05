#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"

typedef struct s_intersection t_intersection;
typedef struct s_ray t_ray;

typedef struct s_shape
{
    void *inhereted;
    char *name;
    t_vec3 color;

    int (*intersect)(struct s_shape *this, t_intersection *intersection);
    int (*does_intersect)(struct s_shape *this, t_ray *ray);
    void (*destructor)(void *this);
} t_shape;

void destruct_shape(t_shape *this);

typedef struct s_shape_plane
{
    t_vec3 position;
    t_vec3 normal;
} t_shape_plane;

t_shape *construct_shape_plane(t_vec3 position, t_vec3 normal, char *name);

typedef struct s_shape_sphere
{
    t_vec3 position;
    float radius;
} t_shape_sphere;

t_shape *construct_shape_sphere(t_vec3 position, float radius, char *name);

#endif // SHAPE_H
