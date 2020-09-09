#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "structs.cl"

//bool intersect_plane(t_plane plane, t_plane_intersection *intersection);
//bool intersect_sphere(t_sphere sphere, t_sphere_intersection *intersection);
//bool intersect_scene_spheres(__global t_sphere *sphere_list, int sphere_count,
//                             t_sphere_intersection *intersection);
//bool intersect_scene_planes(__global t_plane *plane_list, int plane_count,
//                            t_plane_intersection *intersection);


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
    intersection->normal = intersection->normal + sphere->position;
    intersection->normal = normalize(intersection->normal);
    intersection->shape = (__global void*)sphere;
    intersection->shape_type = SHAPE_SPHERE;
    return (true);
}

bool intersect(__global t_sphere *sphere_list, int sphere_count,
                __global t_plane *plane_list, int plane_count,
                __global t_shape *shape_list, int shape_count,
                t_intersection *intersection)
{
    t_intersection local_intersection;
    local_intersection.dist = 0.0f;
    intersection->dist = 0.0f;
    intersection->shape = NULL;
    int index;
    t_shape shape;

    local_intersection.ray = intersection->ray;
    shape = shape_list[0];
    if(shape.type == SHAPE_PLANE)
        intersect_plane(&plane_list[shape.index], intersection);
    else
        if (shape.type == SHAPE_SPHERE)
        intersect_sphere(&sphere_list[shape.index], intersection);
    index = 1;
    while(index < shape_count)
    {
        shape = shape_list[index];
        local_intersection.dist = 0.0f;
        if(shape.type == SHAPE_PLANE)
            intersect_plane(&plane_list[shape.index], &local_intersection);
        else if (shape.type == SHAPE_SPHERE)
            intersect_sphere(&sphere_list[shape.index], &local_intersection);
        if(intersection->dist > local_intersection.dist)
            *intersection = local_intersection;
        index++;
    }
    if(intersection->shape_type == SHAPE_NONE)
        return (false);
    return (true);
}

#endif