#include "camera.h"

void look_at(struct s_camera *this, t_vec3 origin, t_vec3 target, t_vec3 up)
{
    t_vec3 vec;

    this->origin = origin;
    vec = vec3_vec3_sub(&target, &origin);
    this->forward = vec3_normalize(&vec);
    vec = vec3_vec3_cross(&this->forward, &up);
    this->right = vec3_normalize(&vec);
    this->up = vec3_vec3_cross(&this->right, &this->forward);
}

t_camera *construct_camera(float fov, float aspect)
{
    t_camera *this;


    SDL_assert((this = malloc(sizeof(t_camera))) != NULL);

    this->height = tanf(fov);
    this->width = this->height * aspect;

    this->look_at = &look_at;
    return (this);
}
