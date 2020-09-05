#include "scene.h"
#include "libft.h"
#include "ray.h"
#include <SDL2/SDL.h>

static void cache_shapes(t_scene *this)
{
    this->cahce_size = double_ll_len(this->shapes);
	t_double_linked_list *itr;
	size_t index;

	free(this->cached_shapes);
    SDL_assert((this->cached_shapes = malloc(sizeof(t_shape *) * this->cahce_size)) != NULL);
	itr = double_ll_back(this->shapes);
	index = 0;
    while(index < this->cahce_size)
	{
        this->cached_shapes[index] = *(t_shape **)itr->content;
		itr = itr->next;
		index++;
	}
}

static void add_shape(t_scene *this, t_shape **shape)
{
    double_ll_push_front((void *)shape, sizeof(t_shape **), &this->shapes, NULL);
}

static void destroy_shape(t_scene *this, char *name)
{
    t_double_linked_list *itr;
    t_shape *shape;

    itr = this->shapes;
    while(itr != NULL)
    {
        shape = *(t_shape **)itr->content;
        if(ft_strequ(shape->name, name))
        {
//            destruct_shape(shape);
            double_ll_pop_element(&itr, NULL);
            if(itr == NULL)
                this->shapes = NULL;
            else
            {
//                itr = double_ll_back(itr);
                this->shapes = double_ll_back(itr);
            }
            return;
        }
        itr = itr->next;
    }
    return;
}

static int  intersect(struct s_scene *this, t_intersection *intersection)
{
    t_shape *current_shape;
    t_intersection local_intersection;
    float min_dist;
    size_t index;

    if(this->cahce_size == 0)
        return (0);
    local_intersection = *intersection;
    current_shape = this->cached_shapes[0];
    current_shape->intersect(current_shape, &local_intersection);
    min_dist = local_intersection.dist;
    local_intersection.shape = current_shape;
    *intersection = local_intersection;
    min_dist = 0.0f;
    index = 1;
    while(index < this->cahce_size)
    {
        current_shape = this->cached_shapes[index];
        current_shape->intersect(current_shape, &local_intersection);
//        printf("%f\n", local_intersection.dist);
        if(local_intersection.dist > min_dist && local_intersection.dist > 0.0f)
        {
            min_dist = local_intersection.dist;
            local_intersection.shape = current_shape;
            *intersection = local_intersection;
        }
        index++;
    }
    if(min_dist == 0.0f)
        return (0);
    else
        return (1);
}

static void list(struct s_scene *this)
{
	t_double_linked_list *itr;
	t_shape *shape;

	itr = this->shapes;
	while(itr != NULL)
	{
        shape = *(t_shape **)(void *)itr->content;
		ft_putstr(shape->name);
		ft_putchar('\n');
		itr = itr->next;
    }
}

t_scene *construct_scene()
{
	t_scene *this;

	SDL_assert((this = malloc(sizeof(t_scene))) != NULL);
	this->cached_shapes = NULL;
	this->shapes = NULL;
    this->cahce_size = 0;

	this->add_shape = &add_shape;
	this->cache_shapes = &cache_shapes;
	this->list = &list;
    this->destroy_shape = &destroy_shape;
    this->intersect = &intersect;

	return (this);
}

void destruct_scene(struct s_scene *this)
{
	t_double_linked_list *itr;

	itr = this->shapes;
	while(itr != NULL)
	{
		destruct_shape((t_shape *)itr->content);
		itr = itr->next;
	}
	double_ll_destroy_whole(&this->shapes, NULL);
}
