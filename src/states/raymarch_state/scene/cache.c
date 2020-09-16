#include "cache.h"
#include "double_linked_list.h"
#include "SDL2/SDL.h"
#include "libft.h"
#include "scene.h"

static void cache_planes(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_plane *plane;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_planes);
	itr = double_ll_back(scene->planes);
	this->plane_cache_size = double_ll_len(scene->planes);
	SDL_assert((this->cached_planes = malloc(sizeof(t_shape_plane) * this->plane_cache_size)) != NULL);
	index = 0;
	while(index < this->plane_cache_size)
	{
		plane = (*(t_shape **)itr->content)->inhereted;
		this->cached_planes[index] = *plane;
		itr = itr->next;
		index++;
	}
}

static void cache_spheres(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_sphere *plane;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_spheres);
	itr = double_ll_back(scene->spheres);
	this->sphere_cache_size = double_ll_len(scene->spheres);
	SDL_assert((this->cached_spheres = malloc(sizeof(t_shape_sphere) * this->sphere_cache_size)) != NULL);
	index = 0;
	while(index < this->sphere_cache_size)
	{
		plane = (*(t_shape **)itr->content)->inhereted;
		this->cached_spheres[index] = *plane;
		itr = itr->next;
		index++;
	}
}

static void cache_aabbs(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_aabb *aabb;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_aabb);
	itr = double_ll_back(scene->aabbs);
	this->aabb_cache_size = double_ll_len(scene->aabbs);
	SDL_assert((this->cached_aabb = malloc(sizeof(t_shape_aabb) * this->aabb_cache_size)) != NULL);
	index = 0;
	while(index < this->aabb_cache_size)
	{
		aabb = (*(t_shape **)itr->content)->inhereted;
		this->cached_aabb[index] = *aabb;
		itr = itr->next;
		index++;
	}
}

static void cache_triangles(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_triangle *triangle;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_triangles);
	itr = double_ll_back(scene->aabbs);
	this->triangle_cache_size = double_ll_len(scene->triangles);
	SDL_assert((this->cached_triangles = malloc(sizeof(t_shape_triangle) * this->triangle_cache_size)) != NULL);
	index = 0;
	while(index < this->triangle_cache_size)
	{
		triangle = (*(t_shape **)itr->content)->inhereted;
		this->cached_triangles[index] = *triangle;
		itr = itr->next;
		index++;
	}
}

static void cache_squares(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_square *square;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_squares);
	itr = double_ll_back(scene->squares);
	this->square_cache_size = double_ll_len(scene->squares);
	SDL_assert((this->cached_squares = malloc(sizeof(t_shape_square) * this->square_cache_size)) != NULL);
	index = 0;
	while(index < this->square_cache_size)
	{
		square = (*(t_shape **)itr->content)->inhereted;
		this->cached_squares[index] = *square;
		itr = itr->next;
		index++;
	}
}

static void cache_disks(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_disk *disk;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_disks);
	itr = double_ll_back(scene->disks);
	this->disk_cache_size = double_ll_len(scene->disks);
	SDL_assert((this->cached_disks = malloc(sizeof(t_shape_disk) * this->disk_cache_size)) != NULL);
	index = 0;
	while(index < this->disk_cache_size)
	{
		disk = (*(t_shape **)itr->content)->inhereted;
		this->cached_disks[index] = *disk;
		itr = itr->next;
		index++;
	}
}

static void cache_cylinders(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_cylinder *cylinder;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_cylinders);
	itr = double_ll_back(scene->cylinders);
	this->cylinder_cache_size = double_ll_len(scene->cylinders);
	SDL_assert((this->cached_cylinders = malloc(sizeof(t_shape_cylinder) * this->cylinder_cache_size)) != NULL);
	index = 0;
	while(index < this->cylinder_cache_size)
	{
		cylinder = (*(t_shape **)itr->content)->inhereted;
		this->cached_cylinders[index] = *cylinder;
		itr = itr->next;
		index++;
	}
}

static void cache_cones(t_cache *this)
{
	t_double_linked_list *itr;
	t_shape_cone *cylinder;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_cones);
	itr = double_ll_back(scene->cones);
	this->cone_cache_size = double_ll_len(scene->cones);
	SDL_assert((this->cached_cones = malloc(sizeof(t_shape_cone) * this->cone_cache_size)) != NULL);
	index = 0;
	while(index < this->cone_cache_size)
	{
		cylinder = (*(t_shape **)itr->content)->inhereted;
		this->cached_cones[index] = *cylinder;
		itr = itr->next;
		index++;
	}
}

static void cache_point_lights(t_cache *this)
{
	t_double_linked_list *itr;
	t_point_light *point_light;
	size_t index;
	t_scene_items *scene;

	scene = this->scene;
	free(this->cached_point_lights);
	itr = double_ll_back(scene->cones);
	this->point_light_cache_size = double_ll_len(scene->point_lights);
	SDL_assert((this->cached_point_lights = malloc(sizeof(t_point_light) * this->point_light_cache_size)) != NULL);
	index = 0;
	while(index < this->point_light_cache_size)
	{
		point_light = (*(t_shape **)itr->content)->inhereted;
		this->cached_point_lights[index] = *point_light;
		itr = itr->next;
		index++;
	}
}

static void cache(t_cache *this)
{
	this->cache_planes(this);
	this->cache_spheres(this);
	this->cache_aabbs(this);
	this->cache_triangles(this);
	this->cache_squares(this);
	this->cache_disks(this);
	this->cache_cylinders(this);
	this->cache_cones(this);
	this->cache_point_lights(this);
}

t_cache *construct_cache(t_scene_items *scene)
{
	t_cache *object;

	SDL_assert((object = malloc(sizeof (t_cache))) != NULL);
	SDL_assert(scene != NULL);
	object->scene = scene;
	object->cached_planes = NULL;
	object->cached_spheres = NULL;
	object->cached_aabb = NULL;
	object->cached_triangles = NULL;
	object->cached_squares = NULL;
	object->cached_disks = NULL;
	object->cached_cylinders = NULL;
	object->cached_cones = NULL;
	object->cached_shapes = NULL;
	object->cached_materials = NULL;
	object->cached_point_lights = NULL;
	object->plane_cache_size = 0;
	object->sphere_cache_size = 0;
	object->aabb_cache_size = 0;
	object->triangle_cache_size = 0;
	object->square_cache_size = 0;
	object->disk_cache_size = 0;
	object->cylinder_cache_size = 0;
	object->cone_cache_size = 0;
	object->shape_cache_size = 0;
	object->material_cache_size = 0;
	object->point_light_cache_size = 0;

	object->cache_planes = &cache_planes;
	object->cache_spheres = &cache_spheres;
	object->cache_aabbs = &cache_aabbs;
	object->cache_triangles = &cache_triangles;
	object->cache_squares = &cache_squares;
	object->cache_disks = &cache_disks;
	object->cache_cylinders = &cache_cylinders;
	object->cache_cones = &cache_cones;
	object->cache_point_lights = &cache_point_lights;
	return (object);
}