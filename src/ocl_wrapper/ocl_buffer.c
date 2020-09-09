#include "ocl_wrapper.h"
#include "libft.h"
#include <stdio.h>

void				ocl_enqueue_write_buffer(cl_command_queue command_queue, cl_mem buffer, size_t size, const void* data)
{
	cl_int err;
	err = clEnqueueWriteBuffer(command_queue, buffer, CL_TRUE, 0, size, data, 0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Cannot write buffer: ");
		ft_putstr(ocl_error_to_string(err));
		ft_putchar('\n');
		exit(0);
	}
}

void				ocl_enqueue_read_buffer(cl_command_queue command_queue, cl_mem buffer, size_t size, void* data)
{
	cl_int err;
	err = clEnqueueReadBuffer(command_queue, buffer, CL_TRUE, 0, size, data, 0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Cannot read buffer: ");
		ft_putstr(ocl_error_to_string(err));
		ft_putchar('\n');
		exit(0);
	}
}
