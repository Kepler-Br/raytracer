#ifndef RAYTRACER_OCL_WRAPPER_H
#define RAYTRACER_OCL_WRAPPER_H

#ifdef __APPLE__
# include <OpenCL/cl.h>
#elif __linux__
# include <CL/cl.h>
#endif

#define OCL_WRAPPER_MAX_COMPUTE_UNITS 0
#define OCL_WRAPPER_MAX_FREQ 1
#define OCL_WRAPPER_FIRST 2

typedef struct	s_ocl_device
{
	cl_platform_id	platform_id;
	cl_device_id	device_id;
	cl_uint			freq;
	cl_uint			compute_units;
}				t_ocl_device;

void				ocl_print_all_information();
void				ocl_print_device_info_full(cl_device_id device);
void				ocl_print_platform_info(cl_platform_id platform);

cl_device_id		ocl_get_device(const int type);

cl_context			ocl_create_context(cl_device_id device);
cl_command_queue	ocl_create_command_queue(cl_context context, cl_device_id device);
cl_kernel			ocl_get_kernel(cl_program program, char *kernel_name);
cl_mem				ocl_create_buffer(cl_context context, cl_mem_flags flags, size_t len, void *data);

cl_program			ocl_load_and_build_program(cl_context context, cl_device_id device, char *program_path);
void				ocl_set_kernel_arg(cl_kernel kernel, cl_uint index, const size_t arg_size, const void *data);

void				ocl_enqueue_write_buffer(cl_command_queue command_queue, cl_mem buffer, size_t size, const void* data);
void				ocl_enqueue_read_buffer(cl_command_queue command_queue, cl_mem buffer, size_t size, void* data);

void 				ocl_enqueue_nd_range_kernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t *global_work_size, const size_t *local_work_size);
void				ocl_finish(cl_command_queue command_queue);

char				*ocl_error_to_string(int error_code);


#endif //RAYTRACER_OCL_WRAPPER_H
