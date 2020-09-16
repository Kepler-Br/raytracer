#include "shape.h"
#include "libft.h"
#include <SDL2/SDL.h>

t_named_material *construct_named_material(t_material *material, char *name)
{
    t_named_material *named;

	SDL_assert(name != NULL);
	SDL_assert(material  != NULL);
    SDL_assert((named = malloc(sizeof (t_named_material))) != NULL);
    named->name = ft_strdup(name);
    named->material = material;
    return (named);
}

void destruct_named_material(t_named_material *material)
{
	free(material->name);
	free(material->material);
}

t_shape *construct_shape(void *shape, int shape_type, char *name)
{
	t_shape *object;

	SDL_assert(shape  != NULL);
	SDL_assert(name  != NULL);
	SDL_assert((object = malloc(sizeof (t_shape))) != NULL);
	SDL_assert((object->name = ft_strdup(name)) != NULL);
	object->shape_type = shape_type;
	object->inhereted = shape;
	return (object);
}

void destruct_shape(t_shape *shape)
{
	free(shape->name);
	free(shape->inhereted);
}
