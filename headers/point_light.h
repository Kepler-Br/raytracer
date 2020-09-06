#ifndef RAYTRACER_POINT_LIGHT_H
#define RAYTRACER_POINT_LIGHT_H

#include "vec3.h"

typedef struct s_point_light
{
	t_vec3 position;
	t_vec3 color;
	char *name;
} t_point_light;

#endif //RAYTRACER_POINT_LIGHT_H
