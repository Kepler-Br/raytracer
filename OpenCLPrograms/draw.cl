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
    color = (float3){(float)screen->image_array[index + 2]/255.0f,
                     (float)screen->image_array[index + 1]/255.0f,
                     (float)screen->image_array[index]/255.0f};
    return (color);
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
}

float3 get_surface_color(t_intersection *intersection, t_scene *scene)
{
    __global t_point_light *point_light;

    point_light = &scene->point_light_list[0];
    t_material material = get_surface_material(intersection, scene);
    float3 intersection_position = get_intersection_position(intersection);
    float dist;
    if(is_point_visible(scene, intersection_position, point_light->position, &dist))
    {
        float dott = dot(intersection->normal, normalize(point_light->position - intersection_position));
        float3 result_color = (float3){point_light->color.x * dott * 1.0f/(dist)*point_light->power,
                                       point_light->color.y * dott * 1.0f/(dist)*point_light->power,
                                       point_light->color.z * dott * 1.0f/(dist)*point_light->power};
        result_color = result_color - (float3){1.0f, 1.0f, 1.0f} - material.albedo;
        result_color = clamp(result_color, 0.0f, 1.0f);
        return (result_color);
    }
    else
        return ((float3){0.0f, 0.0f, 0.0f});
}

float3 get_direct_light_contribution(t_intersection *intersection, t_scene *scene)
{
    __global t_point_light *point_light;

    point_light = &scene->point_light_list[0];
    float3 intersection_position = get_intersection_position(intersection);
    float dist;
    if(is_point_visible(scene, intersection_position, point_light->position, &dist))
    {
        float dott = dot(intersection->normal, normalize(point_light->position - intersection_position));
        float3 result_color = (float3){point_light->color.x * dott * 1.0f/(dist)*point_light->power,
                                       point_light->color.y * dott * 1.0f/(dist)*point_light->power,
                                       point_light->color.z * dott * 1.0f/(dist)*point_light->power};
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
    const int max_indirect_rays = 4;

    intersection.ray = ray;
    intersection.ray.max_dist = 500.0f;
    if(intersect(scene, &intersection))
    {
        // float3 surface_color = get_surface_color(&intersection, scene);
        float3 direct_light_contribution = get_direct_light_contribution(&intersection, scene);
        float3 indirect_light_contribution = (float3){0.0f, 0.0f, 0.0f};
        float3 result;
        // if(total_GI_rays == 0)
        //     ;
        // else
        int index = max_indirect_rays;
        t_material mat = get_surface_material(&intersection, scene);
        while(index > 0)
        {
            float r1 = randf(random);
            // r1 = clamp(r1, 0.0f, 1.0f);
            float r2 = randf(random);
            // r2 = clamp(r2, 0.0f, 1.0f);
            float3 sample = rand_point_on_hemisphere(r1, r2);
            float3 world_sample = to_world_coordinates(intersection.normal, sample);
            t_intersection bounce_intersection;
            bounce_intersection.ray.direction = world_sample;
            bounce_intersection.ray.max_dist = MAX_RAY_DIST;
            bounce_intersection.ray.origin = get_intersection_position(&intersection) + world_sample*0.0001f;
            
            if(intersect(scene, &bounce_intersection))
            {
                
                // float3 sample_color = get_surface_color(&bounce_intersection, scene);
                float3 sample_color = get_direct_light_contribution(&bounce_intersection, scene) * ((float3){1.0f, 1.0f, 1.0f} - get_surface_material(&bounce_intersection, scene).albedo);
                // if(sample_color.x == NAN || sample_color.y == NAN || sample_color.z == NAN)

                // printf("%f\n", sample_color.y);
                // float3 sample_color = (float3){1.0f, 1.0f, 1.0f} - get_surface_material(&bounce_intersection, scene).albedo;
                
                // if(mat.is_emissive)
                // {
                //     bounce_color += surface_color1;
                // }
                // else
                // {
                    // sample_color = dot(world_sample, intersection.normal) * sample_color;//*1.0f/get_intersection_position(&bounce_intersection);
                    sample_color = r1 * sample_color;//*1.0f/get_intersection_position(&bounce_intersection);
                    indirect_light_contribution += sample_color;
                // }
            }
            else
                indirect_light_contribution += r1 * (float3){0.0f, 0.0f, 0.0f};
            index--;
        }
        const float pdf =  (1.0f / (2.0f * M_PI_F));
        indirect_light_contribution /= (float)max_indirect_rays/pdf;
        indirect_light_contribution = clamp(indirect_light_contribution, 0.0f, 1.0f);
        direct_light_contribution *= mat.albedo;
        direct_light_contribution = clamp(direct_light_contribution, 0.0f, 1.0f);
        result = (indirect_light_contribution + direct_light_contribution) / M_PI_F;
        
        float3 prev_radiance = get_pixel(screen);
        result = lerp(prev_radiance, result, 1.0f/2.0f);
        result = clamp(result, 0.0f, 1.0f);
        // indirect_light_contribution = clamp(indirect_light_contribution, 0.0f, 1.0f);
        set_pixel(screen, result);
    }
    else
        set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
}
#endif