#ifndef DRAW_H
#define DRAW_H

#include "intersection.cl"
#include "structs.cl"


void set_pixel(t_screen *screen, float3 color);
void draw_sphere(t_sphere sphere, t_scene *scene, t_screen *screen);
void draw_plane(t_plane plane, t_scene *scene, t_screen *screen);
void draw_scene(t_scene *scene, t_screen *screen, t_ray ray);


void set_pixel(t_screen *screen, float3 color)
{
    const uint color_count = 4;

    uint index = (screen->geometry.x * screen->current_y + screen->current_x) * color_count;
    screen->image_array[index] = (char)(color.z*255);
    screen->image_array[index + 1] = (char)(color.y*255);
    screen->image_array[index + 2] = (char)(color.x*255);
}


void draw_sphere(t_sphere sphere, t_scene *scene, t_screen *screen)
{
    int material_index = sphere.material_index;
    float3 color = scene->material_list[material_index].absorb_color;
    set_pixel(screen, color);
}

void draw_plane(t_plane plane, t_scene *scene, t_screen *screen)
{
    int material_index = plane.material_index;
    float3 color = scene->material_list[material_index].absorb_color;
    set_pixel(screen, color);
}


void draw_scene(t_scene *scene, t_screen *screen, t_ray ray)
{
    t_intersection intersection;
    __global t_sphere *sphere;
    __global t_plane *plane;
    __global t_material *material;
    int material_index;

    intersection.ray = ray;
    if(intersect(scene, &intersection))
    {
        if(intersection.shape_type == SHAPE_PLANE)
        {
            plane = (__global t_plane *)intersection.shape;
            material_index = plane->material_index;
            material = &scene->material_list[material_index];
        }
        if(intersection.shape_type == SHAPE_SPHERE)
        {
            sphere = (__global t_sphere *)intersection.shape;
            material_index = sphere->material_index;
            material = &scene->material_list[material_index];
        }
        set_pixel(screen, material->absorb_color);
    }
    else
        set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
}
#endif