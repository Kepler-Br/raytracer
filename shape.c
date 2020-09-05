#include "shape.h"
#include "ray.h"
#include "libft.h"
#include <SDL2/SDL.h>

static int plane_intersect(t_shape *this, t_intersection *intersection)
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

static int plane_does_intersect(t_shape *this, t_ray *ray)
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

static void destruct_shape_plane(void *this)
{
}

t_shape *construct_shape_plane(t_vec3 position, t_vec3 normal, char *name)
{
    t_shape *shape;
    t_shape_plane *plane;

    SDL_assert((plane = malloc(sizeof (t_shape_plane))) != NULL);
    SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);
	SDL_assert((shape->name = ft_strdup(name)) != NULL);

    plane->normal = normal;
    plane->position = position;
    shape->color = (t_vec3){{1.0f, 0.0f, 1.0f}};
    shape->inhereted = (void *)plane;

    shape->intersect = &plane_intersect;
    shape->does_intersect = &plane_does_intersect;
    shape->destructor = &destruct_shape_plane;
    return (shape);
}


static void destruct_shape_sphere(void *this)
{
}


static int sphere_intersect(t_shape *this, t_intersection *intersection)
{
    t_shape_sphere *str;

    str = (t_shape_sphere *)this->inhereted;
    t_ray local_ray = intersection->ray;
//    local_ray.origin = vec3_vec3_sub(&local_ray.origin, &str->position);

//    float a = local_ray.direction.x*local_ray.direction.x + local_ray.direction.y*local_ray.direction.y + local_ray.direction.z*local_ray.direction.z;
//    float b = 2.0f * vec3_vec3_dot(&local_ray.direction, &local_ray.origin);
//    float c = local_ray.origin.x*local_ray.origin.x + local_ray.origin.y*local_ray.origin.y + local_ray.origin.z*local_ray.origin.z - str->radius*str->radius;

//    float discriminant = b*b - 4 * a * c;

//    if (discriminant < 0.0f)
//    {
//        return (0);
//    }

//    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
//    float t2 = (-b + sqrtf(discriminant)) / (2 * a);
//    if (t1 > MIN_RAY_DIST && t1 < intersection->dist)
//        intersection->dist = t1;
//    else if (t2 > MIN_RAY_DIST && t2 < intersection->dist)
//        intersection->dist = t2;
//    else
//        return (0);

//    intersection->shape = this;

//    return (0);

    float t0, t1;
    t_vec3 L = vec3_vec3_sub(&str->position, &local_ray.origin);
    float tca = vec3_vec3_dot(&L, &local_ray.direction);
    if(tca < 0)
        return (0);
    double d2 = vec3_vec3_dot(&L, &L) - tca * tca;
    if( d2 > str->radius*str->radius) {
        return (0);
    }

    double thc = sqrtf(str->radius*str->radius -d2);
    t0 = tca - thc;
    t1 = tca + thc;

    if(t0 > t1) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }

    if(t0 < 0){
        t0 = t1;
        if( t0 < 0) {
            return (0);
        }
    }

    intersection->dist = t0;
//    result.normal = glm::normalize((ray.origin + ray.direction * t0) - _origin);
//    intersection->position = ray.origin + ray.direction * t0;
//    intersection->ray. = local_ray.calc(&local_ray, t0);
    return (1);
}

static int sphere_does_intersect(t_shape *this, t_ray *ray)
{
    t_shape_sphere *str;

    str = (t_shape_sphere *)this->inhereted;
    // Transform ray so we can consider origin-centred sphere
    t_ray local_ray = *ray;
    local_ray.origin = vec3_vec3_sub(&local_ray.origin, &str->position);

    // Calculate quadratic coefficients
    float a = local_ray.direction.x*local_ray.direction.x + local_ray.direction.y*local_ray.direction.y + local_ray.direction.z*local_ray.direction.z;
    float b = 2.0f * vec3_vec3_dot(&local_ray.direction, &local_ray.origin);
    float c = local_ray.origin.x*local_ray.origin.x + local_ray.origin.y*local_ray.origin.y + local_ray.origin.z*local_ray.origin.z - str->radius*str->radius;

    // Check whether we intersect
    float discriminant = b*b - 4 * a * c;

    if (discriminant < 0.0f)
    {
        return (0);
    }

    // Find two points of intersection, t1 close and t2 far
    float t1 = (-b - sqrtf(discriminant)) / (2 * a);
    if (t1 > MIN_RAY_DIST && t1 < ray->max_dist)
        return (1);

    float t2 = (-b + sqrtf(discriminant)) / (2 * a);
    if (t2 > MIN_RAY_DIST && t2 < ray->max_dist)
        return (1);

    return (0);
}


t_shape *construct_shape_sphere(t_vec3 position, float radius, char *name)
{
    t_shape *shape;
    t_shape_sphere *sphere;

    SDL_assert((sphere = malloc(sizeof (t_shape_sphere))) != NULL);
    SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);
    SDL_assert((shape->name = ft_strdup(name)) != NULL);

    sphere->radius = radius;
    sphere->position = position;
    shape->color = (t_vec3){{1.0f, 0.0f, 1.0f}};
    shape->inhereted = (void *)sphere;

    shape->intersect = &sphere_intersect;
    shape->does_intersect = &sphere_does_intersect;
    shape->destructor = &destruct_shape_sphere;
    return (shape);
}



void destruct_shape(t_shape *this)
{
	free(this->name);
	this->destructor(this->inhereted);
}
