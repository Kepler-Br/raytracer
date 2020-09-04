#ifndef RENDER_MASK_H
#define RENDER_MASK_H

#ifdef __APPLE__
# include <OpenCL/cl.h>
#elif __linux__
# include <CL/cl.h>
#endif

char *create_render_mask(const size_t size, const int pobability_black);

#endif // RENDER_MASK_H
