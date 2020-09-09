#include "ocl_wrapper.h"
#include "libft.h"
#include <stdio.h>

cl_context			ocl_create_context(cl_device_id device)
{
	cl_int err;
	cl_context context;

	context = clCreateContext(0, 1, &device, NULL, NULL, &err);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Failed to create context: ");
		ft_putstr(ocl_error_to_string(err));
		ft_putchar('\n');
		exit(0);
	}
	return (context);
}

cl_command_queue	ocl_create_command_queue(cl_context context, cl_device_id device)
{
	cl_int err;
	cl_command_queue queue;

	queue = clCreateCommandQueue(context, device, 0, &err);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Failed to create command queue: ");
		ft_putstr(ocl_error_to_string(err));
		ft_putchar('\n');
		exit(0);
	}
	return (queue);
}

cl_kernel			ocl_get_kernel(cl_program program, char *kernel_name)
{
	cl_int err;
	cl_kernel kernel;

	kernel = clCreateKernel(program, kernel_name, &err);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Failed to get kernel: ");
		ft_putstr(ocl_error_to_string(err));
		ft_putchar('\n');
		exit(0);
	}
	return (kernel);
}

cl_mem				ocl_create_buffer(cl_context context, cl_mem_flags flags, size_t len, void *data)
{
	cl_int err;
	cl_mem mem_array_obj;

	mem_array_obj = clCreateBuffer(context, flags, len, data, &err);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Cannot create buffer: ");
		ft_putstr(ocl_error_to_string(err));
		ft_putchar('\n');
		exit(0);
	}
	 return (mem_array_obj);
}
