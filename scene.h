#ifndef SCENE_H
#define SCENE_H

#include "shape.h"
#include "double_linked_list.h"

typedef struct s_scene
{
    t_shape **cached_shapes;
	t_double_linked_list *shapes;
    size_t cahce_size;

	void (*cache_shapes)(struct s_scene *this);
    void (*add_shape)(struct s_scene *this, t_shape **shape);
    void (*destroy_shape)(struct s_scene *this, char *name);
    int  (*intersect)(struct s_scene *this, t_intersection *intersection);
	void (*list)(struct s_scene *this);
} t_scene;

t_scene *construct_scene();
void destruct_scene(struct s_scene *this);

#endif // SCENE_H
