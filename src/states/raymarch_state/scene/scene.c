#include "scene.h"
#include "libft.h"
#include "ray.h"
#include <SDL2/SDL.h>



static int private_destroy_shape_by_name(char *name, t_double_linked_list **shape_list)
{
    t_double_linked_list *itr;
    t_shape *shape;

    itr = *shape_list;
    while(itr != NULL)
    {
        shape = *(t_shape **)itr->content;
        if(ft_strequ(shape->name, name))
        {
            destruct_shape(itr->content);
            free(itr->content);
            double_ll_pop_element(&itr, NULL);
            if(itr == NULL)
                (*shape_list) = NULL;
            else
                (*shape_list) = double_ll_back(itr);
            return 1;
        }
        itr = itr->next;
    }
    return (0);
}

static void destroy_shape(t_scene_items *this, char *name)
{
    if(private_destroy_shape_by_name(name, &this->planes))
        return;
    if(private_destroy_shape_by_name(name, &this->spheres))
        return;
    if(private_destroy_shape_by_name(name, &this->point_lights))
        return;
}

static void destroy_similar_shapes(struct s_scene_items *this, char *name)
{
    while(private_destroy_shape_by_name(name, &this->planes));
    while(private_destroy_shape_by_name(name, &this->spheres));
    while(private_destroy_shape_by_name(name, &this->point_lights));
}

static void private_list_shape_list(t_double_linked_list *shape_list)
{
    t_shape *shape;

    while(shape_list != NULL)
    {
        shape = *(t_shape **)(void *)shape_list->content;
        ft_putstr("\t-");
        ft_putstr(shape->name);
        ft_putchar('\n');
        shape_list = shape_list->next;
    }
}

static void private_list_materials(t_double_linked_list *material_list)
{
    t_named_material *material;

    while(material_list != NULL)
    {
        material = *(t_named_material **)(void *)material_list->content;
        ft_putstr("\t-");
        ft_putstr(material->name);
        ft_putchar('\n');
        material_list = material_list->next;
    }
}

static void list(t_scene_items *this)
{
    ft_putstr("Planes: \n");
    private_list_shape_list(this->planes);
	ft_putstr("Spheres: \n");
	private_list_shape_list(this->spheres);
	ft_putstr("AABBs: \n");
	private_list_shape_list(this->aabbs);
	ft_putstr("Triangles: \n");
	private_list_shape_list(this->triangles);
	ft_putstr("Squares: \n");
	private_list_shape_list(this->squares);
	ft_putstr("Disks: \n");
	private_list_shape_list(this->disks);
	ft_putstr("Cylinders: \n");
	private_list_shape_list(this->cylinders);
	ft_putstr("Cones: \n");
	private_list_shape_list(this->cones);
    ft_putstr("Point lights: \n");
    private_list_shape_list(this->point_lights);
    ft_putstr("Materials: \n");
    private_list_materials(this->materials);
}

void add_material(struct s_scene_items *this, t_material *material, char *name)
{
    t_named_material *named_material;

    SDL_assert((named_material = malloc(sizeof(t_named_material))) != NULL);
    named_material->name = ft_strdup(name);
    named_material->material = material;
    double_ll_push_front((void *)&named_material, sizeof(t_named_material **), &this->materials, NULL);
}

cl_int get_material_index(t_scene_items *this, char *name)
{
    t_double_linked_list *itr;
    cl_int index;
    t_named_material *named_material;

    itr = this->materials;
    index = 0;
    while(itr != NULL)
    {
        named_material = *(t_named_material **)itr->content;
        if(ft_strequ(name, named_material->name))
            return (index);
        index++;
        itr = itr->next;
    }
    return (0);
}

void destroy_material(t_scene_items *this, char *name)
{
    t_double_linked_list *itr;
    t_named_material *named_material;

    itr = this->materials;
    while(itr != NULL)
    {
        named_material = *(t_named_material **)itr->content;
        if(ft_strequ(named_material->name, name))
        {
            destruct_named_material(named_material);
            free(itr->content);
            double_ll_pop_element(&itr, NULL);
            if(itr == NULL)
                this->materials = NULL;
            else
                this->materials = double_ll_back(itr);
            return;
        }
        itr = itr->next;
    }
}

void add_shape(t_scene_items *this, void *shape, int shape_type, char *name)
{
	t_shape *named_shape;

	SDL_assert((named_shape = malloc(sizeof(t_shape))) != NULL);
	SDL_assert((named_shape->name = ft_strdup(name)) != NULL);
	named_shape->inhereted = shape;
	named_shape->shape_type = shape_type;
	if(shape_type == SHAPE_PLANE)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->planes, NULL);
	else if(shape_type == SHAPE_SPHERE)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->spheres, NULL);
	else if(shape_type == SHAPE_AABB)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->aabbs, NULL);
	else if(shape_type == SHAPE_TRIANGLE)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->triangles, NULL);
	else if(shape_type == SHAPE_SQUARE)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->squares, NULL);
	else if(shape_type == SHAPE_DISK)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->disks, NULL);
	else if(shape_type == SHAPE_CYLINDER)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->cylinders, NULL);
	else if(shape_type == SHAPE_CONE)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->cones, NULL);
	else if(shape_type == SHAPE_POINT_LIGHT)
		double_ll_push_front((void *)&named_shape, sizeof(t_shape **), &this->point_lights, NULL);
}

t_scene_items *construct_scene_items()
{
    t_scene_items *this;

    SDL_assert((this = malloc(sizeof(t_scene_items))) != NULL);
	this->planes = NULL;
	this->spheres = NULL;
	this->aabbs = NULL;
	this->triangles = NULL;
	this->squares = NULL;
	this->disks = NULL;
	this->cylinders = NULL;
	this->cones = NULL;
	this->point_lights = NULL;
	this->materials = NULL;
    this->destroy_shape = &destroy_shape;
    this->destroy_similar_shapes = &destroy_similar_shapes;
    this->add_material = &add_material;
    this->destroy_material = &destroy_material;
    this->get_material_index = &get_material_index;
    this->add_shape = &add_shape;
    this->list = &list;
    return (this);
}

static void private_destroy_shape_lambda(void *content)
{
	t_shape *shape;

	shape = (t_shape *)content;
	destruct_shape(shape);
	free(shape);
}

static void private_destroy_all_shapes(t_scene_items *this)
{
	double_ll_destroy_whole(&this->planes, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->spheres, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->aabbs, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->triangles, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->squares, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->disks, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->cylinders, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->cones, &private_destroy_shape_lambda);
	double_ll_destroy_whole(&this->point_lights, &private_destroy_shape_lambda);
}

void destruct_scene_items(struct s_scene_items *this)
{
    t_double_linked_list *itr;

	private_destroy_all_shapes(this);
    itr = this->materials;
    while(itr != NULL)
    {
        destruct_named_material((t_named_material *)itr->content);
        free(itr->content);
        itr = itr->next;
    }
    double_ll_destroy_whole(&this->materials, NULL);
}