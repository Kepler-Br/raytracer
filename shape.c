#include "shape.h"
#include "ray.h"
#include <SDL2/SDL.h>

int plane_intersect(t_shape *this, t_intersection *intersection)
{
    t_shape_plane *str;
    t_vec3 pos_sub;

    str = (t_shape_plane *)this->inhereted;
    // First, check if we intersect
    float dot_normal = vec3_vec3_dot(&intersection->ray.direction, &str->normal);

    if (dot_normal == 0.0f)
    {
        // We just assume the ray is not embedded in the plane
        return (0);
    }

    // Find point of intersection
    pos_sub = vec3_vec3_sub(&str->position, &intersection->ray.origin);
    float point_distance = vec3_vec3_dot(&pos_sub, &str->normal) / dot_normal;

    if (point_distance <= MIN_RAY_DIST || point_distance >= intersection->ray.max_dist)
    {
        // Outside relevant range
        return (0);
    }

    intersection->dist = point_distance;
    intersection->shape = this;

    return (1);
}

int plane_does_intersect(t_shape *this, t_ray *ray)
{
    t_shape_plane *str;
    t_vec3 pos_sub;

    str = (t_shape_plane *)this->inhereted;
    // First, check if we intersect
    float dot_normal = vec3_vec3_dot(&ray->direction, &str->normal);
    if (dot_normal == 0.0f)
    {
        // We just assume the ray is not embedded in the plane
        return (0);
    }
    // Find point of intersection
    pos_sub = vec3_vec3_sub(&str->position, &ray->origin);
    float point_distance = vec3_vec3_dot(&pos_sub, &str->normal) / dot_normal;

    if (point_distance <= MIN_RAY_DIST || point_distance >= ray->max_dist)
    {
        // Outside relevant range
        return (0);
    }
    return (1);
}

t_shape *construct_shape_plane(t_vec3 position, t_vec3 normal)
{
    t_shape *shape;
    t_shape_plane *plane;

    SDL_assert((plane = malloc(sizeof (t_shape_plane))) != NULL);
    SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);

    plane->normal = normal;
    plane->position = position;
    plane->color = (t_ivec3){{1.0f, 0.0f, 1.0f}};

    shape->inhereted = (void *)plane;
    shape->intersect = &plane_intersect;
    shape->does_intersect = &plane_does_intersect;
    return (shape);
}

t_shape *destroy_shape_plane(t_shape *shape)
{
    free(shape->inhereted);
    free(shape);
}
