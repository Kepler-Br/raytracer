#ifndef SCENE_H
#define SCENE_H

#include "shape.h"

typedef struct s_scene
{
    t_shape *shapes;

    void (*cache_shapes)(struct s_scene *this);
} t_scene;

#endif // SCENE_H
