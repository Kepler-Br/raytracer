#include "scene.h"
#include "libft.h"
#include "ray.h"
#include <SDL2/SDL.h>

static void private_cache_spheres(t_scene_items *this)
{
    t_double_linked_list *itr;
    t_shape_sphere *sphere;
    size_t index;

    free(this->cached_spheres);
    this->sphere_cache_size = double_ll_len(this->spheres);
    SDL_assert((this->cached_spheres = malloc(sizeof(t_shape *) * this->sphere_cache_size)) != NULL);
    itr = double_ll_back(this->spheres);
    index = 0;
    while(index < this->sphere_cache_size)
    {
        sphere = (*(t_shape **)itr->content)->inhereted;
        this->cached_spheres[index] = *sphere;
        itr = itr->next;
        index++;
    }
}

static void private_cache_planes(t_scene_items *this)
{
    t_double_linked_list *itr;
    t_shape_plane *plane;
    size_t index;

    free(this->cached_planes);
    this->plane_cache_size = double_ll_len(this->planes);
    SDL_assert((this->cached_planes = malloc(sizeof(t_shape *) * this->plane_cache_size)) != NULL);
    itr = double_ll_back(this->planes);
    index = 0;
    while(index < this->plane_cache_size)
    {
        plane = (*(t_shape **)itr->content)->inhereted;
        this->cached_planes[index] = *plane;
        itr = itr->next;
        index++;
    }
}

static void private_cache_point_lights(t_scene_items *this)
{
    t_double_linked_list *itr;
    t_point_light *point_light;
    size_t index;

    free(this->cached_point_lights);
    this->point_light_cache_size = double_ll_len(this->point_lights);
    SDL_assert((this->cached_point_lights = malloc(sizeof(t_shape *) * this->point_light_cache_size)) != NULL);
    itr = double_ll_back(this->point_lights);
    index = 0;
    while(index < this->point_light_cache_size)
    {
        point_light = (*(t_shape **)itr->content)->inhereted;
        this->cached_point_lights[index] = *point_light;
        itr = itr->next;
        index++;
    }
}

static void private_cache_shapes(t_scene_items *this, int shape_type)
{
    if(shape_type == SHAPE_SPHERE)
        private_cache_spheres(this);
    else if(shape_type == SHAPE_PLANE)
        private_cache_planes(this);
    else if(shape_type == SHAPE_POINT_LIGHT)
        private_cache_point_lights(this);
}

static void private_cache_materials(t_scene_items *this)
{
    t_double_linked_list *itr;
    t_named_material *named_material;
    size_t index;

    free(this->cached_materials);
    this->material_cache_size = double_ll_len(this->materials);
    SDL_assert((this->cached_materials = malloc(sizeof(t_shape *) * this->material_cache_size)) != NULL);
    itr = double_ll_back(this->materials);
    index = 0;
    while(index < this->material_cache_size)
    {
        named_material = *(t_named_material **)itr->content;
        this->cached_materials[index] = *named_material->material;
        itr = itr->next;
        index++;
    }
}

static void cache(t_scene_items *this, int shape_type)
{
    if (shape_type == SHAPE_POINT_LIGHT)
        private_cache_shapes(this, SHAPE_POINT_LIGHT);
    else if (shape_type == SHAPE_SPHERE)
        private_cache_shapes(this, SHAPE_SPHERE);
    else if (shape_type == SHAPE_PLANE)
        private_cache_shapes(this, SHAPE_PLANE);
}

void cache_materials(struct s_scene_items *this)
{
    private_cache_materials(this);
}

static void cache_full(t_scene_items *this)
{
    private_cache_shapes(this, SHAPE_POINT_LIGHT);
    private_cache_shapes(this, SHAPE_SPHERE);
    private_cache_shapes(this, SHAPE_PLANE);
}

static void add_point_light(t_scene_items *this, t_vec3 position, t_vec3 color, char *name)
{
//	t_point_light *point_light;
//
//	SDL_assert((point_light = malloc(sizeof(t_point_light))) != NULL);
//	point_light->name = ft_strdup(name);
//	point_light->position = position;
//	point_light->color = color;
//	double_ll_push_front((void *)&point_light, sizeof(t_point_light **), &this->point_lights, NULL);
}

static void add_shape(t_scene_items *this, t_shape **shape, int shape_type)
{
    if(shape_type == SHAPE_POINT_LIGHT)
        double_ll_push_front((void *)shape, sizeof(t_shape **), &this->point_lights, NULL);
    else if (shape_type == SHAPE_SPHERE)
        double_ll_push_front((void *)shape, sizeof(t_shape **), &this->spheres, NULL);
    else if (shape_type == SHAPE_PLANE)
        double_ll_push_front((void *)shape, sizeof(t_shape **), &this->planes, NULL);
}

static void add_sphere(t_scene_items *this, t_shape_sphere *sphere, char *name)
{
    t_shape *shape;

    SDL_assert((shape = malloc(sizeof(t_shape))) != NULL);
    shape->shape_type = SHAPE_SPHERE;
    shape->name = ft_strdup(name);
    shape->inhereted = (void *)sphere;
    double_ll_push_front((void *)&shape, sizeof(t_shape **), &this->spheres, NULL);
}

void add_plane(struct s_scene_items *this, t_shape_plane *plane, char *name)
{
    t_shape *shape;

    SDL_assert((shape = malloc(sizeof(t_shape))) != NULL);
    shape->shape_type = SHAPE_PLANE;
    shape->name = ft_strdup(name);
    shape->inhereted = (void *)plane;
    double_ll_push_front((void *)&shape, sizeof(t_shape **), &this->planes, NULL);
}

static void private_update_cache_planes(t_scene_items *this)
{
    size_t cache_len;
    t_double_linked_list *itr;
    t_shape_plane *plane;
    size_t index;

    cache_len = double_ll_len(this->planes);
    if(cache_len != this->plane_cache_size)
    {
        this->cache(this, SHAPE_PLANE);
        return;
    }
    index = 0;
    while(index < cache_len)
    {
        plane = (*(t_shape **)itr->content)->inhereted;
        this->cached_planes[index] = *plane;
        index++;
    }
}

static void private_update_cache_spheres(t_scene_items *this)
{
    size_t cache_len;
    t_double_linked_list *itr;
    t_shape_sphere *sphere;
    size_t index;

    cache_len = double_ll_len(this->spheres);
    if(cache_len != this->sphere_cache_size)
    {
        this->cache(this, SHAPE_SPHERE);
        return;
    }
    index = 0;
    while(index < cache_len)
    {
        sphere = (*(t_shape **)itr->content)->inhereted;
        this->cached_spheres[index] = *sphere;
        index++;
    }
}

static void private_update_cache_point_ligths(t_scene_items *this)
{
    size_t cache_len;
    t_double_linked_list *itr;
    t_point_light *point_light;
    size_t index;

    cache_len = double_ll_len(this->point_lights);
    if(cache_len != this->point_light_cache_size)
    {
        this->cache(this, SHAPE_POINT_LIGHT);
        return;
    }
    index = 0;
    while(index < cache_len)
    {
        point_light = (*(t_shape **)itr->content)->inhereted;
        this->cached_point_lights[index] = *point_light;
        index++;
    }
}

static void update_cache(t_scene_items *this, int shape_type)
{
    if(shape_type == SHAPE_PLANE)
        private_update_cache_planes(this);
    else if(shape_type == SHAPE_SPHERE)
        private_update_cache_spheres(this);
    else if(shape_type == SHAPE_POINT_LIGHT)
        private_update_cache_point_ligths(this);
}

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
    private_destroy_shape_by_name(name, &this->planes);
    private_destroy_shape_by_name(name, &this->spheres);
    private_destroy_shape_by_name(name, &this->point_lights);
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

static void list(t_scene_items *this)
{
	ft_putstr("Shapes: \n");
    ft_putstr("Planes: \n");
    private_list_shape_list(this->planes);
    ft_putstr("Spheres: \n");
    private_list_shape_list(this->spheres);
    ft_putstr("Point lights: \n");
    private_list_shape_list(this->point_lights);
    ft_putstr("Materials: \n");
    private_list_shape_list(this->materials);

}

void add_material(struct s_scene_items *this, t_material *material, char *name)
{
    t_named_material *named_material;

    SDL_assert((named_material = malloc(sizeof(t_named_material))) != NULL);
    named_material->name = ft_strdup(name);
    named_material->material = material;
    double_ll_push_front((void *)&named_material, sizeof(t_named_material **), &this->point_lights, NULL);
}
void destroy_material(struct s_scene_items *this, char *name)
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
            free(named_material->name);
            free(named_material->material);
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

t_scene_items *construct_scene_items()
{
	t_scene_items *this;

	SDL_assert((this = malloc(sizeof(t_scene_items))) != NULL);
    this->cached_materials = NULL;
    this->cached_planes = NULL;
	this->cached_point_lights = NULL;
	this->cached_spheres = NULL;

    this->planes = NULL;
    this->spheres = NULL;
    this->point_lights = NULL;
    this->materials = NULL;

    this->plane_cache_size = 0;
    this->sphere_cache_size = 0;
    this->point_light_cache_size = 0;
    this->material_cache_size = 0;

    this->cache = &cache;
    this->cache_full = &cache_full;
//    this->add_shape = &add_shape;
    this->add_sphere = &add_sphere;
    this->add_plane = &add_plane;
    this->destroy_shape = destroy_shape;
    this->cache_materials = &cache_materials;
    this->destroy_similar_shapes = &destroy_similar_shapes;
    this->update_cache = &update_cache;
    this->add_material = &add_material;
    this->destroy_material = &destroy_material;
    this->list = &list;
	return (this);
}

static void private_destroy_shape(t_double_linked_list **list)
{
    t_double_linked_list *itr;

    itr = *list;
    while(itr != NULL)
    {
        destruct_shape((t_shape *)itr->content);
        free(itr->content);
        itr = (*list)->next;
    }
    double_ll_destroy_whole(list, NULL);
}

void destruct_scene_items(struct s_scene_items *this)
{
	t_double_linked_list *itr;

	itr = this->planes;
    private_destroy_shape(&this->planes);
    private_destroy_shape(&this->spheres);
    private_destroy_shape(&this->point_lights);
    itr = this->materials;
    while(itr != NULL)
    {
        destruct_named_material((t_named_material *)itr->content);
        free(itr->content);
        itr = itr->next;
    }
    double_ll_destroy_whole(&this->materials, NULL);
}