#include "ocl_wrapper.h"
#include "tools.h"
#include <stdio.h>

static void build_program(cl_program program, cl_device_id device, char *program_path)
{
	cl_int err;
	char *buf;
	size_t info_size;

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		printf("Failed to build program(Error code: %d): %s\n", err, program_path);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, NULL, NULL, &info_size);
		if(info_size != 0)
		{
			buf = malloc(sizeof(char)*info_size);
			clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, info_size,buf,NULL);
			printf("%s\n", buf);
		}
		else
			printf("No logs provided.\n");
		exit(1);
	}
}

cl_program			ocl_load_and_build_program(cl_context context, cl_device_id device, char *program_path)
{
	cl_program program;
	cl_int err;

	const char *program_text = read_file(program_path);
	if(program_text == NULL)
	{
		printf("Cannot open program file: %s", program_path);
		exit(0);
	}
	program = clCreateProgramWithSource(context, 1, (const char **)&program_text, NULL, &err);
	if(err != CL_SUCCESS)
	{
		printf("Create program from source(Error code: %d): %s", err, program_path);
		exit(0);
	}
	build_program(program, device, program_path);
	return (program);
}

void				ocl_set_kernel_arg(cl_kernel kernel, cl_uint index, const size_t arg_size, const void *data)
{
	cl_int err;

	err = clSetKernelArg(kernel, index, arg_size, data);
	if(err != CL_SUCCESS)
	{
		printf("Cannot set kernel argument. Error code: %d", err);
		exit(0);
	}
}