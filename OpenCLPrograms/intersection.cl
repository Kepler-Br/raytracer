#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "structs.cl"

bool intersect_plane(__global t_plane *plane, t_intersection *intersection);
bool intersect_sphere(__global t_sphere *sphere, t_intersection *intersection);
bool intersect(t_scene *scene, t_intersection *intersection);
bool is_point_visible(t_scene *scene, float3 start, float3 end, float *dist);
float3 get_intersection_position(t_intersection *intersection);


float3 get_intersection_position(t_intersection *intersection)
{
    return (intersection->dist * intersection->ray.direction + intersection->ray.origin);
}

bool intersect_plane(__global t_plane *plane, t_intersection *intersection)
{
    float3 pos_sub;
    intersection->shape_type = SHAPE_NONE;
    // First, check if we intersect
    float dot_normal = dot(intersection->ray.direction, plane->normal);

    if (dot_normal == 0.0f)
    {
        // We just assume the ray is not embedded in the plane
        return (false);
    }

    // Find point of intersection
    pos_sub = plane->position - intersection->ray.origin;
    float point_distance = dot(pos_sub, plane->normal) / dot_normal;

    if (point_distance <= MIN_RAY_DIST || point_distance >= intersection->ray.max_dist)
    {
        return (false);
    }

    intersection->dist = point_distance;
    intersection->shape = (__global void *)plane;
    intersection->normal = plane->normal;
    intersection->shape_type = SHAPE_PLANE;

    return (true);
}

bool intersect_sphere(__global t_sphere *sphere, t_intersection *intersection)
{
    t_ray local_ray = intersection->ray;
    intersection->shape_type = SHAPE_NONE;
    float t0, t1;
    float3 L = sphere->position - local_ray.origin;
    float tca = dot(L, local_ray.direction);
    if(tca < 0)
        return (false);
    float d2 = dot(L, L) - tca * tca;
    if( d2 > sphere->radius*sphere->radius) {
        return (false);
    }

    double thc = sqrt(sphere->radius*sphere->radius -d2);
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
            return (false);
        }
    }
    intersection->dist = t0;
    intersection->normal = local_ray.direction * t0;
    intersection->normal = local_ray.origin + intersection->normal;
    intersection->normal = intersection->normal - sphere->position;
    intersection->normal = normalize(intersection->normal);
    intersection->shape = (__global void*)sphere;
    intersection->shape_type = SHAPE_SPHERE;
    return (true);
}

bool is_point_visible(t_scene *scene, float3 start, float3 end, float *dist)
{
    t_ray ray;
    t_intersection intersection;
    float dist_towards_point;

    ray.origin = end;
    ray.direction = normalize(start - end);
    ray.max_dist = 100.0f;
	intersection.ray = ray;
	dist_towards_point = length(end - start);

	if(intersect(scene, &intersection))
	{
		if(intersection.dist + 0.0005f >= dist_towards_point)
		{
			*dist = dist_towards_point;
			return (true);
		}
		return (false);
	}
    return (false);
}

bool intersect(t_scene *scene, t_intersection *intersection)
{
    t_intersection local_intersection;
    int index;
    t_shape shape;

    local_intersection.dist = 0.0f;
    intersection->dist = 0.0f;
    local_intersection.ray = intersection->ray;
    intersection->shape_type = SHAPE_NONE;
    index = 0;
    while(index < scene->shape_count)
    {
        shape = scene->shape_list[index];
        if(shape.type == SHAPE_PLANE)
            intersect_plane(&scene->plane_list[shape.index], &local_intersection);
        else if (shape.type == SHAPE_SPHERE)
            intersect_sphere(&scene->sphere_list[shape.index], &local_intersection);
        if(intersection->dist > local_intersection.dist || (intersection->shape_type == SHAPE_NONE && local_intersection.shape_type != SHAPE_NONE))
            *intersection = local_intersection;
        index++;
    }
    if(intersection->shape_type == SHAPE_NONE)
        return (false);
    return (true);
}

#endif