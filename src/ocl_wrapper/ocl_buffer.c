#include "ocl_wrapper.h"
#include <stdio.h>

void				ocl_enqueue_write_buffer(cl_command_queue command_queue, cl_mem buffer, size_t size, const void* data)
{
	cl_int err;
	err = clEnqueueWriteBuffer(command_queue, buffer, CL_TRUE, 0, size, data, 0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		printf("Cannot write buffer. Error code: %d", err);
		exit(0);
	}
}

void				ocl_enqueue_read_buffer(cl_command_queue command_queue, cl_mem buffer, size_t size, void* data)
{
	cl_int err;
	err = clEnqueueReadBuffer(command_queue, buffer, CL_TRUE, 0, size, data, 0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		printf("Cannot write buffer. Error code: %d", err);
		exit(0);
	}
}
