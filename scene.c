#include "scene.h"
#include <SDL2/SDL.h>

static void cache_shapes(t_scene *this)
{
	const size_t total_shapes = double_ll_len(this->shapes);
	t_double_linked_list *itr;
	size_t index;

	free(this->cached_shapes);
	SDL_assert((this->cached_shapes = malloc(sizeof(t_shape *) * total_shapes)) != NULL);
	itr = double_ll_back(this->shapes);
	index = 0;
	while(index < total_shapes)
	{
		this->cached_shapes[index] = (t_shape *)itr->content;
		itr = itr->next;
		index++;
	}
}

static void add_shape(t_scene *this, t_shape *shape)
{
	double_ll_push_front(&shape, sizeof(t_scene*), &this->shapes, NULL);
}

t_scene *construct_scene()
{
	t_scene *this;

	SDL_assert((this = malloc(sizeof(t_scene))) != NULL);
	this->cached_shapes = NULL;
	this->shapes = NULL;
	this->add_shape = &add_shape;
	this->cache_shapes = &cache_shapes;
}