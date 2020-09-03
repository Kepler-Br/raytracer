#include "camera.h"

static t_ray make_ray(t_camera *this, t_vec2 *screen_point)
{
    t_vec3 direction;
    t_vec3 vec;
    t_vec3 vec2;
    float num;

    num = screen_point->x * this->width;
    vec = vec3_scalar_mul(&this->right, &num);
    vec = vec3_vec3_sum(&this->forward, &vec);

    num = screen_point->y * this->height;
    vec2 = vec3_scalar_mul(&this->up, &num);

    direction = vec3_vec3_sum(&vec, &vec2);
    direction = vec3_normalize(&direction);
    return (construct_ray(this->origin, direction, MAX_RAY_DIST));
}

t_camera *construct_camera(t_vec3 origin, t_vec3 target, t_vec3 up, float fov, float aspect)
{
    t_camera *this;
    t_vec3 vec;

    SDL_assert((this = malloc(sizeof(t_camera))) != NULL);

    this->origin = origin;
    this->up = up;
    vec = vec3_vec3_sub(&target, &origin);
    this->forward = vec3_normalize(&vec);
    vec = vec3_vec3_cross(&this->forward, &up);
    this->right = vec3_vec3_cross(&this->right, &this->forward);
    this->height = tanf(fov);
    this->width = this->height * aspect;

    this->make_ray = &make_ray;
    return (this);
}
