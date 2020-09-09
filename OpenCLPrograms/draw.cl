#ifndef DRAW_H
#define DRAW_H

#include "intersection.cl"
#include "structs.cl"

void set_pixel(int x, int y, float3 color, __global char *image_array, int2 screen_geometry);
void draw_sphere(t_sphere sphere,
                 __global t_material *material_list, int material_count,
                int x, int y, __global char *image_array, int2 screen_geometry);
void draw_plane(t_plane plane,
                __global t_material *material_list, int material_count,
                int x, int y, __global char *image_array, int2 screen_geometry);
void draw_scene(__global t_sphere *sphere_list, int sphere_count,
        __global t_plane *plane_list, int plane_count,
        __global t_point_light *point_light_list, int point_light_count,
        __global t_material *material_list, int material_count,
        int x, int y, __global char *image_array, int2 screen_geometry,
        t_ray ray);


void set_pixel(int x, int y, float3 color, __global char *image_array, int2 screen_geometry)
{
    const uint color_count = 4;

//    char3 byte_color = (char3){(char)color.z*255, (char)color.y*255, (char)color.x*255};
    uint index = (screen_geometry.x * y + x) * color_count;
    image_array[index] = (char)color.x*255;
    image_array[index + 1] = (char)color.y*255;
    image_array[index + 2] = (char)color.z*255;
}


void draw_sphere(t_sphere sphere,
                 __global t_material *material_list, int material_count,
int x, int y, __global char *image_array, int2 screen_geometry)
{
    int material_index = sphere.material_index;
    float3 color = material_list[material_index].absorb_color;
    set_pixel(x, y, color, image_array, screen_geometry);
}

void draw_plane(t_plane plane,
                __global t_material *material_list, int material_count,
                int x, int y, __global char *image_array, int2 screen_geometry)
{
    int material_index = plane.material_index;
    float3 color = material_list[material_index].absorb_color;
    set_pixel(x, y, color, image_array, screen_geometry);
}

void draw_scene(__global t_sphere *sphere_list, int sphere_count,
        __global t_plane *plane_list, int plane_count,
        __global t_point_light *point_light_list, int point_light_count,
        __global t_material *material_list, int material_count,
int x, int y, __global char *image_array, int2 screen_geometry,
t_ray ray)
{
    t_sphere_intersection sphere_intersection;
    t_plane_intersection plane_intersection;
    bool was_sphere_intersected;
    bool was_plane_intersected;

    sphere_intersection.ray = ray;
    plane_intersection.ray = ray;
    was_sphere_intersected = intersect_scene_spheres(sphere_list,
                                                     sphere_count,
                                                     &sphere_intersection);
    was_plane_intersected = intersect_scene_planes(plane_list,
                                                   plane_count,
                                                   &plane_intersection);
    if(was_sphere_intersected && was_plane_intersected)
    {
        if(sphere_intersection.dist < plane_intersection.dist)
            draw_sphere(sphere_intersection.shape, material_list,
                        material_count, x, y, image_array, screen_geometry);
        else
            draw_plane(plane_intersection.shape, material_list,
                        material_count, x, y, image_array, screen_geometry);
    }
    else if(was_sphere_intersected)
    {
        draw_sphere(sphere_intersection.shape, material_list,
                    material_count, x, y, image_array, screen_geometry);
    }
    else if (was_plane_intersected)
    {
        draw_plane(plane_intersection.shape, material_list,
                    material_count, x, y, image_array, screen_geometry);
    }
    else
    {
        set_pixel(x, y, (float3){0.0f, 0.0f, 0.0f},
                    image_array, screen_geometry);
    }
}

#endif