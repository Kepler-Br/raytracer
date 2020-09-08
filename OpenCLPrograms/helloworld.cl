#define MAX_RAY_DIST 1000000.0f
#define MIN_RAY_DIST 0.00001f

typedef struct
{
    float3 origin;
    float3 forward;
    float3 up;
    float3 right;
    float height;
    float width;
} t_camera;

typedef struct
{
    float3 origin;
    float3 direction;
    float max_dist;
} t_ray;

typedef struct
{
    float3 position;
    float radius;
    uint material_index;
} t_sphere;

typedef struct
{
    float3 position;
    float3 normal;
} t_plane;

typedef struct
{
    t_ray ray;
    float dist;
    float3 normal;
    t_sphere shape;
} t_sphere_intersection;

typedef struct
{
    t_ray ray;
    float dist;
    float3 normal;
    t_plane shape;
} t_plane_intersection;

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

void set_pixel(int x, int y, float3 color, __global char *image_array, int2 screen_geometry)
{
    const uint color_count = 4;

//    char3 byte_color = (char3){(char)color.z*255, (char)color.y*255, (char)color.x*255};
    uint index = (screen_geometry.x * y + x) * color_count;
    image_array[index] = (char)color.x*255;
    image_array[index + 1] = (char)color.y*255;
    image_array[index + 2] = (char)color.z*255;
}

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

__kernel void main_kernel(__global char *image_array, __global int *random_array,
                          int random_array_size, int2 screen_geometry, int random_number,
                          int skip_percentage, t_camera cam, __global t_sphere *sphere_list,
                          int sphere_count, __global t_plane *plane_list, int plane_count)
{

    int image_x = get_global_id(0);
    int image_y = get_global_id(1);
//    bool should_skip_frame = random_array[((screen_geometry.x * image_y + image_x)*random_number)%random_array_size]%100 < skip_percentage;
//    if(should_skip_frame)
//    {
//        set_pixel(image_x, image_y, (float3){0.0f, 0.0f, 0.0f}, image_array, screen_geometry);
//        return;
//    }
    float2 screen_coordinates = (float2){2.0f * image_x / screen_geometry.x - 1.0f,
                                         2.0f * image_y / screen_geometry.y - 1.0f};
    t_ray ray = make_ray(&cam, screen_coordinates);
//    t_plane_intersection intersection;
    t_sphere_intersection intersection;
    intersection.ray = ray;

//if(intersect_sphere(sphere_list[0], &intersection))
//    if(image_x == 1 && image_y == 0)
//    {
//        printf("sphere_count %d\n", sphere_count);
//    }
    if(intersect_scene_spheres(sphere_list, sphere_count, &intersection))
//    if(intersect_plane(plane_list[0],&intersection))
//    if(intersection.dist > 0.0f)
        set_pixel(image_x, image_y, (float3){1.0f, 1.0f, 1.0f}, image_array, screen_geometry);
    else
        set_pixel(image_x, image_y, (float3){1.0f, 0.0f, 1.0f}, image_array, screen_geometry);

}