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
    cl_float height;
    cl_float width;
} t_camera;

t_camera *construct_camera(cl_float fov, cl_float aspect);
void camera_look_at(t_camera *camera, t_vec3 origin, t_vec3 target, t_vec3 up);

#endif // CAMERA_H
