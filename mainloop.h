#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "check_timer.h"
#include "framebuffer.h"
#include "camera.h"
#include "render_mask.h"
#include "scene.h"
#include "input_manager.h"

typedef struct s_mainloop
{
    t_check_timer *check_timer;
    t_framebuffer *framebuffer;
    t_sdl_instance *sdl_instance;
    t_input_manager *input_manager;
    t_camera *camera;
    t_scene *scene;
    char *render_mask;

    float deltatime;
	size_t frame_count;
	float time_since_start;
    uint max_fps;

    int is_running;

    void (*run)(struct s_mainloop *this);
    void (*calc_deltatime)(struct s_mainloop *this);
    void (*limit_fps)(struct s_mainloop *this);

    void (*update)(struct s_mainloop *this);
    void (*late_update)(struct s_mainloop *this);
    void (*fixed_update)(struct s_mainloop *this);

    void (*prerender)(struct s_mainloop *this);
    void (*render)(struct s_mainloop *this);
    void (*postrender)(struct s_mainloop *this);

    void (*input)(struct s_mainloop *this);
    void (*stop)(struct s_mainloop *this);
} t_mainloop;

t_mainloop *construct_mainloop(t_ivec2 resolution, const char * const title);

#endif // MAINLOOP_H
