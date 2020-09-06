#ifndef SCENE_H
#define SCENE_H

#include "shape.h"
#include "point_light.h"
#include "double_linked_list.h"

typedef struct s_scene
{
    t_shape **cached_shapes;
	t_double_linked_list *shapes;
	t_point_light **cached_point_lights;
	t_double_linked_list *point_lights;
	size_t shape_cahce_size;
	size_t point_light_cahce_size;
    t_vec3 ambient_color;

	void (*cache)(struct s_scene *this);
	void (*add_shape)(struct s_scene *this, t_shape **shape);
	void (*add_point_light)(struct s_scene *this, t_vec3 position, t_vec3 color, char *name);
    void (*destroy_shape)(struct s_scene *this, char *name);
    int  (*intersect)(struct s_scene *this, t_intersection *intersection);
	int (*is_point_visible)(struct s_scene *this, t_vec3 start, t_vec3 end, float *dist);
	void (*list)(struct s_scene *this);
} t_scene;

t_scene *construct_scene();
void destruct_scene(struct s_scene *this);

#endif // SCENE_H
