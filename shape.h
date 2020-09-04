#ifndef SHAPE_H
#define SHAPE_H

#include "linear_alg.h"

typedef struct s_intersection t_intersection;
typedef struct s_ray t_ray;

typedef struct s_shape
{
    void *inhereted;
    char *name;
    t_vec3 color;

    int (*intersect)(struct s_shape *this, t_intersection *intersection);
    int (*does_intersect)(struct s_shape *this, t_ray *ray);
} t_shape;

typedef struct s_shape_plane
{
    t_vec3 position;
    t_vec3 normal;
} t_shape_plane;

t_shape *construct_shape_plane(t_vec3 position, t_vec3 normal, char *name);
void destroy_shape_plane(t_shape *shape);

#endif // SHAPE_H
