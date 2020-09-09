#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "structs.cl"

bool intersect_plane(t_plane plane, t_plane_intersection *intersection);
bool intersect_sphere(t_sphere sphere, t_sphere_intersection *intersection);
bool intersect_scene_spheres(__global t_sphere *sphere_list, int sphere_count,
                             t_sphere_intersection *intersection);
bool intersect_scene_planes(__global t_plane *plane_list, int plane_count,
                            t_plane_intersection *intersection);


bool intersect_plane(t_plane plane, t_plane_intersection *intersection)
{
    float3 pos_sub;

    // First, check if we intersect
    float dot_normal = dot(intersection->ray.direction, plane.normal);


    if (dot_normal == 0.0f)
    {
        // We just assume the ray is not embedded in the plane
        return (false);
    }

    // Find point of intersection
    pos_sub = plane.position - intersection->ray.origin;
    float point_distance = dot(pos_sub, plane.normal) / dot_normal;

    if (point_distance <= MIN_RAY_DIST || point_distance >= intersection->ray.max_dist)
    {
        return (false);
    }

    intersection->dist = point_distance;
    intersection->shape = plane;
    intersection->normal = plane.normal;

    return (true);
}

bool intersect_sphere(t_sphere sphere, t_sphere_intersection *intersection)
{
    t_ray local_ray = intersection->ray;

    float t0, t1;
    float3 L = sphere.position - local_ray.origin;
    float tca = dot(L, local_ray.direction);
    if(tca < 0)
        return (false);
    float d2 = dot(L, L) - tca * tca;
    if( d2 > sphere.radius*sphere.radius) {
        return (false);
    }

    double thc = sqrt(sphere.radius*sphere.radius -d2);
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
    intersection->normal = intersection->normal + sphere.position;
    intersection->normal = normalize(intersection->normal);
    return (true);
}

bool intersect_scene_spheres(__global t_sphere *sphere_list, int sphere_count, t_sphere_intersection *intersection)
{
    t_sphere_intersection local_intersection;
    int i;

    intersection->dist = 0;
    local_intersection = *intersection;
    i = 0;
    while(i < sphere_count)
    {
        if(intersect_sphere(sphere_list[i], &local_intersection) &&
            local_intersection.dist > intersection->dist)
        *intersection = local_intersection;

        i++;
    }
    if(intersection->dist == 0)
        return (false);
    return (true);
}

bool intersect_scene_planes(__global t_plane *plane_list, int plane_count,
        t_plane_intersection *intersection)
{
    t_plane_intersection local_intersection;
    int i;

    intersection->dist = 0;
    local_intersection = *intersection;

    i = 0;
    while(i < plane_count)
    {
        if(intersect_plane(plane_list[i], &local_intersection) &&
            local_intersection.dist > intersection->dist)
        *intersection = local_intersection;
        i++;
    }
    if(intersection->dist == 0)
        return (false);
    return (true);
}

#endif