#include "camera.h"

void camera_look_at(t_camera *camera, t_vec3 origin, t_vec3 target, t_vec3 up)
{
    t_vec3 vec;

    camera->origin = origin;
    vec = vec3_vec3_sub(&target, &origin);
    vec = (t_vec3){{target.x - origin.x, target.y - origin.y, target.z - origin.z}};
    camera->forward = vec3_normalize(&vec);
    vec = vec3_vec3_cross(&camera->forward, &up);
    camera->right = vec3_normalize(&vec);
    camera->up = vec3_vec3_cross(&camera->right, &camera->forward);
}

t_camera *construct_camera(cl_float fov, cl_float aspect)
{
    t_camera *this;

    SDL_assert((this = malloc(sizeof(t_camera))) != NULL);
    this->height = tanf(fov);
    this->width = this->height * aspect;
    return (this);
}
