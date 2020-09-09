#include "./OpenCLPrograms/structs.cl"
#include "./OpenCLPrograms/intersection.cl"
#include "./OpenCLPrograms/draw.cl"

t_ray make_ray(t_camera *cam, float2 screen_point)
{
    float3 direction;
    float3 vec;
    float3 vec2;
    float num;

    num = screen_point.x * cam->width;
    vec = cam->right * num;
    vec = cam->forward + vec;

    num = screen_point.y * cam->height;
    vec2 = cam->up * num;

    direction = vec + vec2;
    direction = normalize(direction);
    return (t_ray){cam->origin, direction, MAX_RAY_DIST};
}


__kernel void main_kernel(
                          __global char *image_array, __global int *random_array,
                          int random_array_size, int2 screen_geometry, int random_number,
                          int skip_percentage, t_camera cam,
                          __global t_sphere *sphere_list, int sphere_count,
                          __global t_plane *plane_list, int plane_count,
                          __global t_point_light *point_light_list, int point_light_count,
                          __global t_material *material_list, int material_count,
                          __global t_shape *shape_list, int shape_count
                          )
{
    int image_x = get_global_id(0);
    int image_y = get_global_id(1);
    float2 screen_coordinates = (float2){2.0f * image_x / screen_geometry.x - 1.0f,
                                         2.0f * image_y / screen_geometry.y - 1.0f};
    t_ray ray = make_ray(&cam, screen_coordinates);
    draw_scene(sphere_list, sphere_count,
               plane_list, plane_count,
               point_light_list, point_light_count,
               material_list, material_count,
               shape_list, shape_count,
               image_x, image_y, image_array, screen_geometry,
               ray);
}