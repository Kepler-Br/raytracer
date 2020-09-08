#ifndef RAYTRACER_STATE_H
#define RAYTRACER_STATE_H

#ifdef __APPLE__
# include <OpenCL/cl.h>
#elif __linux__
# include <CL/cl.h>
#endif

typedef struct	s_state
{
	void			*instance_struct;

	void			(*pre_render)(struct s_state *this);
	void			(*render)(struct s_state *this);
	void			(*post_render)(struct s_state *this);

	void			(*update)(struct s_state *this, float deltatime);
	void			(*late_update)(struct s_state *this);
	void			(*fixed_update)(struct s_state *this, float deltatime);

	void 			(*input)(struct s_state *this);

	void 			(*on_stop)(struct s_state *this);
	void			(*destructor)(struct s_state *this);
}				t_state;

#endif //RAYTRACER_STATE_H
