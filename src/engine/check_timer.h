#ifndef CHECK_TIMER_H
#define CHECK_TIMER_H

#include <SDL2/SDL.h>


typedef struct s_check_timer
{
    float time_passed;
    uint32_t last_time;

    float (*check)(struct s_check_timer *this);
} t_check_timer;

t_check_timer *construct_check_timer();

#endif // CHECK_TIMER_H
