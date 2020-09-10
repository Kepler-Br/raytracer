#include "./OpenCLPrograms/structs.cl"
#include "./OpenCLPrograms/intersection.cl"
#include "./OpenCLPrograms/draw.cl"
#include "./OpenCLPrograms/tools.cl"

t_ray make_ray(t_camera *cam, float2 screen_point);
void construct_scene(t_scene *scene,
                    __global t_sphere *sphere_list, int sphere_count,
                    __global t_plane *plane_list, int plane_count,
                    __global t_point_light *point_light_list, int point_light_count,
                    __global t_material *material_list, int material_count,
                    __global t_shape *shape_list, int shape_count);

void construct_screen(t_screen *screen, __global char *image_array,
                    int2 screen_geometry, int x, int y);

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


void construct_scene(t_scene *scene,
                    __global t_sphere *sphere_list, int sphere_count,
                    __global t_plane *plane_list, int plane_count,
                    __global t_point_light *point_light_list, int point_light_count,
                    __global t_material *material_list, int material_count,
                    __global t_shape *shape_list, int shape_count)
{
    scene->sphere_list = sphere_list;
    scene->sphere_count = sphere_count;

    scene->plane_list = plane_list;
    scene->plane_count = plane_count;

    scene->point_light_list = point_light_list;
    scene->point_light_count = point_light_count;

    scene->material_list = material_list;
    scene->material_count = material_count;

    scene->shape_list = shape_list;
    scene->shape_count = shape_count;
}

void construct_screen(t_screen *screen, __global char *image_array,
                    int2 screen_geometry, int x, int y)
{
    screen->current_x = x;
    screen->current_y = y;
    screen->geometry = screen_geometry;
    screen->image_array = image_array;
}

void construct_random(t_random *random, __global int *random_array,
            int size, int host_random_number, int2 global_id)
{
    random->array = random_array;
    random->size = size;
    random->host_random_number = host_random_number;
    random->global_id = global_id;
    random->iteration = 0;
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
    t_screen screen;
    t_scene scene;
    t_random random;
    int image_x = get_global_id(0);
    int image_y = get_global_id(1);
    float2 screen_coordinates;
    
    image_x = get_global_id(0);
    image_y = get_global_id(1);
    screen_coordinates = (float2){2.0f * image_x / screen_geometry.x - 1.0f,
                                         2.0f * image_y / screen_geometry.y - 1.0f};
    construct_scene(&scene, sphere_list, sphere_count,
                           plane_list, plane_count,
                           point_light_list, point_light_count,
                           material_list, material_count,
                           shape_list, shape_count);
    construct_screen(&screen, image_array, screen_geometry, image_x, image_y);
    construct_random(&random, random_array, random_array_size, random_number,
                    (int2){image_x, image_y});
    t_ray ray = make_ray(&cam, screen_coordinates);
    // if(image_x == 0 && image_y == 0)
    // {
    //     printf("%f ", randf(&random));
    //     printf("%f ", randf(&random));
    //     printf("%f ", randf(&random));
    //     printf("%f ", randf(&random));
    //     printf("%f\n", randf(&random));
    // }
    draw_scene(&scene, &screen, &random, ray);
}