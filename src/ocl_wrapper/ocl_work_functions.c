#include "ocl_wrapper.h"
#include <stdio.h>

void 				ocl_enqueue_nd_range_kernel(cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t *global_work_size, const size_t *local_work_size)
{
	cl_int err;

	err = clEnqueueNDRangeKernel(command_queue, kernel, work_dim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		printf("Cannot enqueue ND Range kernel. Error code: %d\n", err);
		exit(0);
	}
}

void				ocl_finish(cl_command_queue command_queue)
{
	cl_int err;
	err = clFinish(command_queue);
	if(err != CL_SUCCESS)
	{
		printf("Cannot finish work. Error code: %d\n", err);
		exit(0);
	}
}