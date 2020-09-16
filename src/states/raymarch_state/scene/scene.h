#ifndef SCENE_H
#define SCENE_H

#include "../shape.h"
#include "double_linked_list.h"

typedef struct s_scene_items
{
    t_double_linked_list *planes;
    t_double_linked_list *spheres;
	t_double_linked_list *aabbs;
	t_double_linked_list *triangles;
	t_double_linked_list *squares;
	t_double_linked_list *disks;
	t_double_linked_list *cylinders;
	t_double_linked_list *cones;
    t_double_linked_list *point_lights;
    t_double_linked_list *materials;

    void (*destroy_shape)(struct s_scene_items *this, char *name);
    void (*destroy_similar_shapes)(struct s_scene_items *this, char *name);
	void (*add_shape)(struct s_scene_items *this, void *shape, int shape_type, char *name);
	void (*add_material)(struct s_scene_items *this, t_material *material, char *name);
	cl_int (*get_material_index)(struct s_scene_items *this, char *name);
    void (*destroy_material)(struct s_scene_items *this, char *name);
	void (*list)(struct s_scene_items *this);
} t_scene_items;

t_scene_items *construct_scene_items();
void destruct_scene_items(struct s_scene_items *this);

#endif // SCENE_H
