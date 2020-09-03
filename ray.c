#include "ray.h"

static t_vec3 calc(t_ray *this, float dist)
{
    static t_vec3 scalar_mul;

    scalar_mul = vec3_scalar_mul(&this->direction, &dist);
    return vec3_vec3_sum(&this->origin, &scalar_mul);
}

t_ray construct_ray(t_vec3 origin, t_vec3 direction, float max_dist)
{
    t_ray this;

    this.origin = origin;
    this.max_dist = max_dist;
    this.direction = direction;

    this.calc = &calc;
    return (this);
}


t_intersection construct_intersection(t_ray ray)
{
    t_intersection this;

    this.dist = 0.0f;
    this.ray = ray;
    this.shape = NULL;
    return (this);
}
