#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "vec2.h"
#include "ray.h"
#include <SDL2/SDL.h>

typedef struct s_camera
{
    t_vec3 origin;
    t_vec3 forward;
    t_vec3 up;
    t_vec3 right;
    float height;
    float width;

    t_ray (*make_ray)(struct s_camera *this, t_vec2 *screen_point);
    void (*look_at)(struct s_camera *this, t_vec3 origin, t_vec3 target, t_vec3 up);
} t_camera;

t_camera *construct_camera(float fov, float aspect);

#endif // CAMERA_H
