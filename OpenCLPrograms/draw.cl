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


float3 get_pixel(t_screen *screen)
{
    const uint color_count = 4;
    float3 color;

    uint index = (screen->geometry.x * screen->current_y + screen->current_x) * color_count;
    color = (float3){(float)screen->image_array[index]/255.0f,
                     (float)screen->image_array[index + 1]/255.0f,
                     (float)screen->image_array[index + 2]/255.0f};
    return (color);
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

t_material get_surface_material(t_intersection *intersection, t_scene *scene)
{
    __global t_sphere *sphere;
    __global t_plane *plane;
    __global t_material *material;
    int material_index;

    
    if(intersection->shape_type == SHAPE_PLANE)
    {
        plane = &scene->plane_list[intersection->shape_index];
        material_index = plane->material_index;
        material = &scene->material_list[material_index];
    }
    if(intersection->shape_type == SHAPE_SPHERE)
    {
        sphere = &scene->sphere_list[intersection->shape_index];
        material_index = sphere->material_index;
        material = &scene->material_list[material_index];
    }
    return (*material);
    // if(material->is_emissive)
    //     return (material->color);
    // float3 intersection_position = get_intersection_position(intersection);
    // float dist;
    // if(is_point_visible(scene, intersection_position, point_light->position, &dist))
    // {
    //     float dott = dot(intersection->normal, normalize(point_light->position - intersection_position));
    //     float3 result_color = (float3){point_light->color.x * dott * 1.0f/(dist)*point_light->power,
    //                                    point_light->color.y * dott * 1.0f/(dist)*point_light->power,
    //                                    point_light->color.z * dott * 1.0f/(dist)*point_light->power};
    //     result_color = result_color - material->color;
    //     result_color = clamp(result_color, 0.0f, 1.0f);
    //     return (result_color);
    // }
    // else
    //     return ((float3){0.0f, 0.0f, 0.0f});
}

float3 get_surface_color(t_intersection *intersection, t_scene *scene)
{
    __global t_point_light *point_light;

    point_light = &scene->point_light_list[0];
    t_material material = get_surface_material(intersection, scene);
    if(material.is_emissive)
        return (material.color);
    float3 intersection_position = get_intersection_position(intersection);
    float dist;
    if(is_point_visible(scene, intersection_position, point_light->position, &dist))
    {
        float dott = dot(intersection->normal, normalize(point_light->position - intersection_position));
        float3 result_color = (float3){point_light->color.x * dott * 1.0f/(dist)*point_light->power,
                                       point_light->color.y * dott * 1.0f/(dist)*point_light->power,
                                       point_light->color.z * dott * 1.0f/(dist)*point_light->power};
        result_color = result_color - material.color;
        result_color = clamp(result_color, 0.0f, 1.0f);
        return (result_color);
    }
    else
        return ((float3){0.0f, 0.0f, 0.0f});
}

float3	lerp(float3 source, float3 target, float t)
{
	return ((float3){(1.0f - t) * source.x + t * target.x,
					 (1.0f - t) * source.y + t * target.y,
					 (1.0f - t) * source.z + t * target.z});
}

void draw_scene(t_scene *scene, t_screen *screen, t_random *random, t_ray ray)
{
    t_intersection intersection;
    __global t_sphere *sphere;
    __global t_plane *plane;
    const int max_indirect_rays = 2;

    intersection.ray = ray;
    intersection.ray.max_dist = 500.0f;
    if(intersect(scene, &intersection))
    {
        float3 surface_color = get_surface_color(&intersection, scene);
        float3 bounce_color;
        // if(total_GI_rays == 0)
        //     ;
        // else
        int index = max_indirect_rays;
        while(index > 0)
        {
            float3 sample = rand_point_on_hemisphere(random);
            float3 world_sample = to_world_coordinates(intersection.normal, sample);
            t_intersection bounce_intersection;
            bounce_intersection.ray.direction = world_sample;
            bounce_intersection.ray.max_dist = MAX_RAY_DIST;
            bounce_intersection.ray.origin = get_intersection_position(&intersection);
            t_material mat;
            if(intersect(scene, &bounce_intersection))
            {
                float3 surface_color1 = get_surface_color(&bounce_intersection, scene);
                // if(mat.is_emissive)
                // {
                //     bounce_color += surface_color1;
                // }
                // else
                // {
                    surface_color1 = dot(world_sample, intersection.normal) * surface_color1;//*1.0f/get_intersection_position(&bounce_intersection);
                    bounce_color += surface_color1;
                // }
            }
            else
                ;
            index--;
        }
        bounce_color /= (float)max_indirect_rays * (1.0f / (2.0f * M_PI_F)); 
        surface_color = bounce_color * surface_color;
        surface_color = clamp(surface_color, 0.0f, 1.0f);
        float3 prev_radiance = get_pixel(screen);
        float3 accumulated_radiance = lerp(prev_radiance, surface_color, 0.1f);
        set_pixel(screen, accumulated_radiance);
    }
    else
        set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
}
#endif