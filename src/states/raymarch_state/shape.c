#include "shape.h"
#include "ray.h"
#include "libft.h"
#include <SDL2/SDL.h>

t_named_material *construct_named_material(char *name, t_material input_material)
{
    t_named_material *named;
    t_material *material;

    SDL_assert((named = malloc(sizeof (t_named_material))) != NULL);
    SDL_assert((material = ft_memdup(&input_material, sizeof(t_material))) != NULL);
    named->name = ft_strdup(name);
    named->material = material;
    return (named);
}

void destruct_named_material(t_named_material *material)
{
    free(material->name);
    free(material->material);
}

static void destruct_shape_point_light(void *this)
{
}

t_shape *construct_shape_point_light(t_vec3 position, t_vec3 color, char *name)
{
    t_shape *shape;
    t_point_light *point_light;

    SDL_assert((point_light = malloc(sizeof (t_point_light))) != NULL);
    SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);
    SDL_assert((shape->name = ft_strdup(name)) != NULL);

    point_light->position = position;
    point_light->color = color;
    shape->inhereted = (void *)point_light;
    shape->shape_type = SHAPE_POINT_LIGHT;
    shape->destructor = &destruct_shape_point_light;
    return (shape);
}

static void destruct_shape_plane(void *this)
{
}

t_shape *construct_shape_plane(t_vec3 position, t_vec3 normal, char *name)
{
    t_shape *shape;
    t_shape_plane *plane;

    SDL_assert((plane = malloc(sizeof (t_shape_plane))) != NULL);
    SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);
	SDL_assert((shape->name = ft_strdup(name)) != NULL);

    plane->normal = vec3_normalize(&normal);
    plane->position = position;
    shape->inhereted = (void *)plane;
    shape->shape_type = SHAPE_PLANE;

    shape->destructor = &destruct_shape_plane;
    return (shape);
}

static void destruct_shape_sphere(void *this)
{
}

t_shape *construct_shape_sphere(t_vec3 position, float radius, char *name)
{
    t_shape *shape;
    t_shape_sphere *sphere;

    SDL_assert((sphere = malloc(sizeof (t_shape_sphere))) != NULL);
    SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);
    SDL_assert((shape->name = ft_strdup(name)) != NULL);

    sphere->radius = radius;
    sphere->position = position;
    shape->inhereted = (void *)sphere;
    shape->shape_type = SHAPE_SPHERE;

    shape->destructor = &destruct_shape_sphere;
    return (shape);
}

static void destruct_shape_cube(void *this)
{
}

t_shape *construct_shape_cube(t_vec3 min, t_vec3 max, char *name)
{
	t_shape *shape;
	t_shape_cube *cube;

	SDL_assert((cube = malloc(sizeof (t_shape_cube))) != NULL);
	SDL_assert((shape = malloc(sizeof (t_shape))) != NULL);
	SDL_assert((shape->name = ft_strdup(name)) != NULL);

	cube->min = min;
	cube->max = max;
	shape->inhereted = (void *)cube;
	shape->shape_type = SHAPE_CUBE;

	shape->destructor = &destruct_shape_cube;
	return (shape);
}

void destruct_shape(t_shape *this)
{
	free(this->name);
	this->destructor(this->inhereted);
}
