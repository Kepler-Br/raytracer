#include "check_timer.h"

static float check(struct s_check_timer *this)
{
    this->time_passed = (float)(SDL_GetTicks() - this->last_time);
    this->last_time = SDL_GetTicks();
    return (this->time_passed);
}

t_check_timer construct_check_timer()
{
    t_check_timer this;

    this.check = &check;
    this.last_time = SDL_GetTicks();
    this.time_passed = 0.0f;
    return (this);
}
