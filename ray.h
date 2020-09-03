#ifndef RAY_H
#define RAY_H

#include "linear_alg.h"

#define MAX_RAY_DIST 1.0e30f
#define MIN_RAY_DIST 0.0001f

typedef struct s_ray
{
    t_vec3 origin;
    t_vec3 direction;
} t_ray;

#endif // RAY_H
