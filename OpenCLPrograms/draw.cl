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
    color = (float3){(float)screen->image_array[index + 2]/254.0f,
                     (float)screen->image_array[index + 1]/254.0f,
                     (float)screen->image_array[index]/254.0f};
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
    t_material mat = get_surface_material(intersection, scene);
    if(mat.is_emissive)
        return (float3){1.0f, 1.0f, 1.0f}*mat.emission_power;
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

float3 gamma_correction(float3 color, float exposure, float gamma)
{
    return (float3)
    {
        pow(color.x * exposure, gamma),
        pow(color.y * exposure, gamma),
        pow(color.z * exposure, gamma)
    };
}

float3 emit_rays(t_intersection *start_intersection, t_scene *scene, t_random *random, int max_rays)
{
    int index = 0;
    float3 contribution_light = (float3){0.0f, 0.0f, 0.0f};
    t_intersection intersection;
    t_material start_material = get_surface_material(start_intersection, scene);

    while(index < max_rays)
    {
        float r1 = randf(random);
        float r2 = randf(random);
        float3 sample = rand_point_on_hemisphere(r1, r2);
        float3 world_sample = to_world_coordinates(start_intersection->normal, sample);
        intersection.ray.direction = world_sample;
        intersection.ray.max_dist = MAX_RAY_DIST;
        intersection.ray.origin = get_intersection_position(start_intersection) + world_sample*0.1f;
        if(intersect(scene, &intersection))
        {
            t_material material = get_surface_material(&intersection, scene);
            float3 sample_color = material.albedo;
            if(material.is_emissive)
                contribution_light += r1*sample_color*material.emission_power*start_material.albedo;
        }
        index++;
    }
    return (contribution_light);
}

void draw_scene(t_scene *scene, t_screen *screen, t_random *random, t_ray ray)
{
    t_intersection intersection;
    __global t_sphere *sphere;
    __global t_plane *plane;
    const int max_indirect_rays = 1;
    const int max_bounce = 2;
    float3 colors[max_bounce];
    t_intersection intersections[max_bounce];

    intersection.ray = ray;
    intersection.ray.max_dist = MAX_RAY_DIST;
    // int i = 0;
    // if(intersect(scene, &intersection))
    // {
    //     int i = 0;
    //     while(i < max_bounce)
    //     {
    //         float r1 = randf(random);
    //         float r2 = randf(random);
    //         float3 sample = rand_point_on_hemisphere(r1, r2);
    //         float3 world_sample = to_world_coordinates(intersection->normal, sample);
    //         intersections[i].ray.origin = get_intersection_position(intersection);
    //         intersections[i].ray.direction = world_sample;
    //         if(intersect(scene, &intersections[i]))
    //         {
    //             emit_rays
    //         }
    //         else
    //             break;
    //         i++;
    //     }
    // }
    // while(i < max_indirect_rays)
    // {
    //     intersections[0] = intersection;
    //     int j = 0;
    //     // while(j < max_bounce)
    //     // {
    //     //     if(intersect(scene, &intersections[j]))
    //     //     {
    //     //         intersections[j+1] = t_intersection;

    //     //     }
    //     //     j++;
    //     // }
    //     i++;
    // }

    if(intersect(scene, &intersection))
    {
        float3 light_contribution = (float3){0.0f, 0.0f, 0.0f};
        int index = max_indirect_rays;
        t_material mat = get_surface_material(&intersection, scene);
        if(mat.is_emissive)
            light_contribution = mat.emission_power*100.0f;
        else
        {
            intersections[0] = intersection;

            light_contribution += emit_rays(&intersection, scene, random, max_indirect_rays/max_bounce);
            light_contribution += emit_rays(&intersection, scene, random, max_indirect_rays);
        }
        const float pdf =  (1.0f / (2.0f * M_PI_F));
        light_contribution /= (float)max_indirect_rays/pdf;
        // light_contribution = clamp(light_contribution, 0.0f, 1.0f);
        // light_contribution *= mat.albedo;
        float3 result;
        result = light_contribution / M_PI_F;
        
        result = gamma_correction(result, 5000.0f, 2.2f);
        float3 prev_radiance = get_pixel(screen);
        // result = lerp(prev_radiance, result, 1.0f/15.0f);
        result = clamp(result, 0.0f, 1.0f);
        set_pixel(screen, result);
    }
    else
        set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
}

// void draw_scene(t_scene *scene, t_screen *screen, t_random *random, t_ray ray)
// {
//     t_intersection intersection;
//     __global t_sphere *sphere;
//     __global t_plane *plane;
//     const int max_indirect_rays = 400;

//     intersection.ray = ray;
//     intersection.ray.max_dist = 500.0f;
//     if(intersect(scene, &intersection))
//     {
//         // float3 surface_color = get_surface_color(&intersection, scene);
//         float3 direct_light_contribution = get_direct_light_contribution(&intersection, scene);
        
//         float3 indirect_light_contribution = (float3){0.0f, 0.0f, 0.0f};
//         float3 result;
//         // if(total_GI_rays == 0)
//         //     ;
//         // else
//         int index = max_indirect_rays;
//         t_material mat = get_surface_material(&intersection, scene);

//         while(index > 0)
//         {
//             float r1 = randf(random);
//             // r1 = clamp(r1, 0.0f, 1.0f);
//             float r2 = randf(random);
//             // r2 = clamp(r2, 0.0f, 1.0f);
//             float3 sample = rand_point_on_hemisphere(r1, r2);
//             float3 world_sample = to_world_coordinates(intersection.normal, sample);
//             t_intersection bounce_intersection;
//             bounce_intersection.ray.direction = world_sample;
//             bounce_intersection.ray.max_dist = MAX_RAY_DIST;
//             bounce_intersection.ray.origin = get_intersection_position(&intersection) + world_sample*0.0001f;
            
//             if(intersect(scene, &bounce_intersection))
//             {
//                 t_material bounce_material = get_surface_material(&bounce_intersection, scene);
//                 float3 sample_color = get_direct_light_contribution(&bounce_intersection, scene) * bounce_material.albedo;
//                 if(bounce_material.is_emissive)
//                 {
//                     // indirect_light_contribution += bounce_material.albedo*bounce_material.emission_power;
//                     indirect_light_contribution += bounce_material.emission_power;
//                 }
//                 else
//                 {
//                     // sample_color = r1 * sample_color;
//                     sample_color = r1* sample_color;
//                     indirect_light_contribution += sample_color;
//                 }
//             }
//             else
//                 indirect_light_contribution += r1 * (float3){0.0f, 0.0f, 0.0f};
//             index--;
//         }
//         const float pdf =  (1.0f / (2.0f * M_PI_F));
//         indirect_light_contribution /= (float)max_indirect_rays/pdf;
//         // indirect_light_contribution = clamp(indirect_light_contribution, 0.0f, 1.0f);
//         direct_light_contribution = clamp(direct_light_contribution, 0.0f, 1.0f);
//         indirect_light_contribution = clamp(indirect_light_contribution, 0.0f, 1.0f);
//         direct_light_contribution *= mat.albedo;
//         indirect_light_contribution *= mat.albedo;
        
//         result = (indirect_light_contribution + direct_light_contribution) / M_PI_F;
        
//         result = gamma_correction(result, 100000.0f, 2.2f);
// // result = clamp(result, 0.0f, 1.0f);
//         float3 prev_radiance = get_pixel(screen);
//         // result = lerp(prev_radiance, result, 1.0f/60.0f);
//         result = clamp(result, 0.0f, 0.5f);
//         set_pixel(screen, result);
//     }
//     else
//         set_pixel(screen, (float3){0.0f, 0.0f, 0.0f});
// }
#endif