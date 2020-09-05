#include "scene.h"
#include "libft.h"
#include "ray.h"
#include <SDL2/SDL.h>

static void private_cache_shapes(t_scene *this)
{
	this->shape_cahce_size = double_ll_len(this->shapes);
	t_double_linked_list *itr;
	size_t index;

	free(this->cached_shapes);
	SDL_assert((this->cached_shapes = malloc(sizeof(t_shape *) * this->shape_cahce_size)) != NULL);
	itr = double_ll_back(this->shapes);
	index = 0;
	while(index < this->shape_cahce_size)
	{
		this->cached_shapes[index] = *(t_shape **)itr->content;
		itr = itr->next;
		index++;
	}
}

static void private_cache_point_lights(t_scene *this)
{
	this->point_light_cahce_size = double_ll_len(this->point_lights);
	t_double_linked_list *itr;
	size_t index;

	free(this->cached_point_lights);
	SDL_assert((this->cached_point_lights = malloc(sizeof(t_point_light *) * this->shape_cahce_size)) != NULL);
	itr = double_ll_back(this->point_lights);
	index = 0;
	while(index < this->point_light_cahce_size)
	{
		this->cached_point_lights[index] = *(t_point_light **)itr->content;
		itr = itr->next;
		index++;
	}
}

static void cache(t_scene *this)
{
	private_cache_shapes(this);
	private_cache_point_lights(this);
}

static void add_point_light(t_scene *this, t_vec3 position, t_vec3 color, char *name)
{
	t_point_light *point_light;

	SDL_assert((point_light = malloc(sizeof(t_point_light))) != NULL);
	point_light->name = ft_strdup(name);
	point_light->position = position;
	point_light->color = color;
	double_ll_push_front((void *)&point_light, sizeof(t_point_light **), &this->point_lights, NULL);
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
}

static int  intersect(struct s_scene *this, t_intersection *intersection)
{
    t_shape *current_shape;
    t_intersection local_intersection;
    float min_dist;
    size_t index;

    if(this->shape_cahce_size == 0)
        return (0);
    local_intersection = *intersection;
    current_shape = this->cached_shapes[0];
    current_shape->intersect(current_shape, &local_intersection);
    local_intersection.shape = current_shape;
    *intersection = local_intersection;
    min_dist = local_intersection.dist;
    index = 1;
    while(index < this->shape_cahce_size)
    {
        current_shape = this->cached_shapes[index];
        current_shape->intersect(current_shape, &local_intersection);
//        printf("%f\n", local_intersection.dist);
        if(local_intersection.dist < min_dist || min_dist == 0.0f)
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

static int is_point_visible(t_scene *this, t_vec3 start, t_vec3 end, float *dist)
{
	t_ray ray = construct_ray(end, vec3_normalize_val(vec3_vec3_sub(&start, &end)), 100.0f);
	t_intersection intersection = construct_intersection(ray);
	float vec_len = vec3_len_val(vec3_vec3_sub_val(end, start));

	if(this->intersect(this, &intersection))
	{
		if(intersection.dist + 0.0005f >= vec_len || intersection.dist == 0.0f)
		{
			*dist = vec_len;
			return (1);
		}
		return (0);
	}
	return (0);
}

static void list(t_scene *this)
{
	t_double_linked_list *itr;
	t_shape *shape;
	t_point_light *point_light;

	ft_putstr("Shapes: \n");
	itr = this->shapes;
	while(itr != NULL)
	{
        shape = *(t_shape **)(void *)itr->content;
		ft_putstr("\t-");
		ft_putstr(shape->name);
		ft_putchar('\n');
		itr = itr->next;
    }
	ft_putstr("Point lights: \n");
	itr = this->point_lights;
	while(itr != NULL)
	{
		point_light = *(t_point_light **)(void *)itr->content;
		ft_putstr("\t-");
		ft_putstr(point_light->name);
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
	this->cached_point_lights = NULL;
	this->point_lights = NULL;
    this->shape_cahce_size = 0;
	this->point_light_cahce_size = 0;
    this->ambient_color = (t_vec3){{0.3f, 0.3f, 0.3f}};

	this->add_shape = &add_shape;
	this->cache = &cache;
	this->list = &list;
    this->destroy_shape = &destroy_shape;
    this->intersect = &intersect;
	this->add_point_light = &add_point_light;
	this->is_point_visible = &is_point_visible;
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
