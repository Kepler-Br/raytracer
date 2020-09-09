#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_RAY_DIST 1000000.0f
#define MIN_RAY_DIST 0.00001f
#define SHAPE_NONE 0
#define SHAPE_PLANE 1
#define SHAPE_SPHERE 2
#define SHAPE_POINT_LIGHT 3

typedef struct
{
    int type;
    int index;
} t_shape;

typedef struct
{
    float3 absorb_color;
    bool is_emissive;
    float3 emissive_color;
} t_material;

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
    uint material_index;
} t_plane;

typedef struct
{
    float3 position;
    float3 emission_color;
} t_point_light;

typedef struct
{
    t_ray ray;
    float dist;
    float3 normal;
    __global void *shape;
    int shape_type;
} t_intersection;


typedef struct
{
    __global t_sphere *sphere_list;
    int sphere_count;
    __global t_plane *plane_list;
    int plane_count;
    __global t_point_light *point_light_list;
    int point_light_count;
    __global t_shape *shape_list;
    int shape_count;
    __global t_material *material_list;
    int material_count;
} t_scene;

typedef struct
{
    int current_x;
    int current_y;
    int2 geometry;
    __global char *image_array;
} t_screen;

#endif