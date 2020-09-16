#include "./OpenCLPrograms/structs.cl"
#include "./OpenCLPrograms/intersection.cl"
#include "./OpenCLPrograms/draw.cl"
#include "./OpenCLPrograms/tools.cl"

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
                    __global t_plane *plane_list, int plane_count,
                    __global t_sphere *sphere_list, int sphere_count,
                    __global t_aabb *aabb_list, int aabb_count,
                    __global t_triangle *triangle_list, int triangle_count,
                    __global t_square *square_list, int square_count,
                    __global t_disk *disk_list, int disk_count,
                    __global t_cylinder *cylinder_list, int cylinder_count,
                    __global t_cone *cone_list, int cone_count,
                    __global t_point_light *point_light_list, int point_light_count,
                    __global t_material *material_list, int material_count,
                    __global t_shape *shape_list, int shape_count
                    )
{

    scene->plane_list = plane_list;
    scene->plane_count = plane_count;

    scene->sphere_list = sphere_list;
    scene->sphere_count = sphere_count;

    scene->aabb_list = aabb_list;
    scene->aabb_count = aabb_count;

    scene->triangle_list = triangle_list;
    scene->triangle_count = triangle_count;

    scene->square_list = square_list;
    scene->square_count = square_count;

    scene->disk_list = disk_list;
    scene->disk_count = disk_count;

    scene->cylinder_list = cylinder_list;
    scene->cylinder_count = cylinder_count;

    scene->cone_list = cone_list;
    scene->cone_count = cone_count;

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
            int size, int2 host_random_number, int2 global_id)
{
    random->array = random_array;
    random->size = size;
    random->host_random_number = host_random_number;
    random->global_id = global_id;
    random->iteration = 0;
    random->state.x = host_random_number.x*(global_id.y+1);
    random->state.c = host_random_number.y*(global_id.x+1);
    // MWC64X_SeedStreams(&random->state, host_random_number.x, 2*(global_id.x+1)*(global_id.y+1));
}

bool should_skip_frame(int skip_percentage, t_random *random)
{
    if(randi(random)%100 > skip_percentage)
        return (true);
    return (false);
}

__kernel void main_kernel(
                          __global char *image_array, __global int *random_array,
                          int random_array_size, int2 screen_geometry, int2 random_number,
                          int skip_percentage, t_camera cam,
                          __global t_plane *plane_list, int plane_count,
                          __global t_sphere *sphere_list, int sphere_count,
                          __global t_aabb *aabb_list, int aabb_count,
                          __global t_triangle *triangle_list, int triangle_count,
                          __global t_square *square_list, int square_count,
                          __global t_disk *disk_list, int disk_count,
                          __global t_cylinder *cylinder_list, int cylinder_count,
                          __global t_cone *cone_list, int cone_count,
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

    construct_random(&random, random_array, random_array_size, random_number,
                    (int2){image_x, image_y});


    screen_coordinates = (float2){2.0f * image_x / screen_geometry.x - 1.0f,
                                  2.0f * image_y / screen_geometry.y - 1.0f};
    construct_scene(&scene, 
                    plane_list, plane_count,
                    sphere_list, sphere_count,
                    aabb_list, aabb_count,
                    triangle_list, triangle_count,
                    square_list, square_count,
                    disk_list, disk_count,
                    cylinder_list, cylinder_count,
                    cone_list, cone_count,
                    point_light_list, point_light_count,
                    material_list, material_count,
                    shape_list, shape_count
                    );
    construct_screen(&screen, image_array, screen_geometry, image_x, image_y);
    // if(should_skip_frame(skip_percentage, &random))
    // {
    //     set_pixel(&screen, (float3){0.0f, 0.0f, 0.0f});
    //     return;
    // }
    t_ray ray = make_ray(&cam, screen_coordinates);
    draw_scene(&scene, &screen, &random, ray);
}