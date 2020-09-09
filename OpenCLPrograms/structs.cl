#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_RAY_DIST 1000000.0f
#define MIN_RAY_DIST 0.00001f

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

typedef struct
{
    float3 position;
    float3 emission_color;
} t_point_light;

#endif