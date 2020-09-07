#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "check_timer.h"
#include "framebuffer.h"
#include "input_manager.h"
#include "state.h"
#include "double_linked_list.h"


typedef struct	s_mainloop
{
    t_check_timer *check_timer;
    t_sdl_instance *sdl_instance;
    t_input_manager *input_manager;
    t_double_linked_list *state_stack;
    t_state *top_state;
	float fixed_update_step_time;


    float deltatime;
	size_t frame_count;
	float time_since_start;
    uint max_fps;

    int is_running;

    void (*run)(struct s_mainloop *this);
    void (*calc_deltatime)(struct s_mainloop *this);
    void (*limit_fps)(struct s_mainloop *this);
    void (*push_state)(struct s_mainloop *this, t_state *state);
    void (*pop_state)(struct s_mainloop *this);
    void (*stop)(struct s_mainloop *this);
}				t_mainloop;

t_mainloop	*construct_mainloop(t_ivec2 resolution, const char *const title);
void		destruct_mainloop(t_mainloop *this);

#endif // MAINLOOP_H
