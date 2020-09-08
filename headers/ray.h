#ifndef RAY_H
#define RAY_H



#define MAX_RAY_DIST 1.0e30f
#define MIN_RAY_DIST 0.0001f

#include "vec3.h"
#include "../src/states/raymarch_state/shape.h"

typedef struct s_ray
{
    t_vec3 origin;
    t_vec3 direction;
    float max_dist;

    t_vec3(*calc)(struct s_ray *this, float dist);
} t_ray;

t_ray construct_ray(t_vec3 origin, t_vec3 direction, float max_dist);

typedef struct s_intersection
{
    t_ray ray;
    float dist;
    t_shape *shape;
    t_vec3 normal;

    int (*is_intersected)(struct s_intersection *this);
    t_vec3 (*position)(struct s_intersection *this);
} t_intersection;

t_intersection construct_intersection(t_ray ray);

#endif // RAY_H
