#ifndef DRAW_H
#define DRAW_H

#include "intersection.cl"
#include "structs.cl"
#include "tools.cl"


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
    float3 color = scene->material_list[material_index].color;
    set_pixel(screen, color);
}

void draw_plane(t_plane plane, t_scene *scene, t_screen *screen)
{
    int material_index = plane.material_index;
    float3 color = scene->material_list[material_index].color;
    set_pixel(screen, color);
}

void draw_scene(t_scene *scene, t_screen *screen, t_random *random, t_ray ray)
{
    t_intersection intersection;
    __global t_sphere *sphere;
    __global t_plane *plane;
    __global t_material *material;
    __global t_point_light *point_light;
    int material_index;

    point_light = &scene->point_light_list[0];
    intersection.ray = ray;
    if(intersect(scene, &intersection))
    {
        if(intersection.shape_type == SHAPE_PLANE)
        {
            // plane = (__global t_plane *)intersection.shape;
            plane = &scene->plane_list[intersection.shape_index];
            material_index = plane->material_index;
            material = &scene->material_list[material_index];
        }
        if(intersection.shape_type == SHAPE_SPHERE)
        {
            // sphere = (__global t_sphere *)intersection.shape;
            sphere = &scene->sphere_list[intersection.shape_index];
            material_index = sphere->material_index;
            material = &scene->material_list[material_index];
        }
        float dist;
        float3 intersection_position = get_intersection_position(&intersection);
        // if(is_point_visible(scene, point_light->position, intersection.ray.origin, &dist))
        if(is_point_visible(scene, intersection_position, point_light->position, &dist))
        {

            float dott = dot(intersection.normal, normalize(point_light->position - intersection_position));
            float3 result_color = (float3){point_light->color.x * dott * 1.0f/(dist)*point_light->power,
                                            point_light->color.y * dott * 1.0f/(dist)*point_light->power,
                                            point_light->color.z * dott * 1.0f/(dist)*point_light->power};
                                             
            // result_color = clamp(result_color, 0.0f, 1.0f);
            result_color = result_color - material->color;
            result_color = clamp(result_color, 0.0f, 1.0f);
            float3 random_direction = rand_point_on_hemisphere(random);
            random_direction = to_world_coordinates(intersection.normal, random_direction);
            intersection.ray.direction = random_direction;
            intersection.ray.max_dist = 10.0f;
            // random_direction += intersection_position;
            
            if(intersect(scene, &intersection))
            {
                intersection_position = get_intersection_position(&intersection);
                if(intersection.shape_type == SHAPE_PLANE)
                {
                    // plane = (__global t_plane *)intersection.shape;
                     plane = &scene->plane_list[intersection.shape_index];
                    material_index = plane->material_index;
                    material = &scene->material_list[material_index];
                }
                else if(intersection.shape_type == SHAPE_SPHERE)
                {
                    // sphere = (__global t_sphere *)intersection.shape;
                    sphere = &scene->sphere_list[intersection.shape_index];
                    material_index = sphere->material_index;
                    material = &scene->material_list[material_index];
                }
                
                if(is_point_visible(scene, intersection_position, point_light->position, &dist))
                {
                    dott = dot(intersection.normal, normalize(point_light->position - intersection_position));
                    float3 bounce_color = (float3){point_light->color.x * dott * 1.0f/(dist)*point_light->power,
                                                point_light->color.y * dott * 1.0f/(dist)*point_light->power,
                                                point_light->color.z * dott * 1.0f/(dist)*point_light->power};
                    bounce_color = bounce_color - material->color;
                    // printf("%p\n", material);
                    bounce_color = clamp(bounce_color, 0.0f, 1.0f);
                    // dott = dot(intersection.ray.direction, );
                    // printf("%f %f %f\n", material->color.x, material->color.y, material->color.z);
                    result_color = (result_color * bounce_color)/1.0f;
                }
            }
            set_pixel(screen, result_color);
            // set_pixel(screen, intersection.normal);
        }
        else
        set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
            // set_pixel(screen, intersection.normal);
    }
    else
        set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
}
#endif