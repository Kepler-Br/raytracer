#include "ocl_wrapper.h"
#include "tools.h"
#include "libft.h"
#include <stdio.h>

static void build_program(cl_program program, cl_device_id device, char *program_path)
{
	cl_int err;
	char *buf;
	size_t info_size;

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Failed to build program: ");
		ft_putendl(ocl_error_to_string(err));
		ft_putendl(program_path);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, NULL, NULL, &info_size);
		if(info_size != 0)
		{
			buf = malloc(sizeof(char)*info_size);
			clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, info_size,buf,NULL);
			ft_putendl(buf);
			free(buf);
		}
		else
			ft_putendl("No logs provided.");
		exit(1);
	}
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, NULL, NULL, &info_size);
	if(info_size != 0)
	{
		buf = malloc(sizeof(char)*info_size);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, info_size,buf,NULL);
		ft_putendl(buf);
		free(buf);
	}
}

cl_program			ocl_load_and_build_program(cl_context context, cl_device_id device, char *program_path)
{
	cl_program program;
	cl_int err;

	char *program_text;
	program_text = read_file(program_path);
	if(program_text == NULL)
	{
		ft_putstr("Cannot open program file: ");
		ft_putendl(program_path);
		exit(0);
	}
	program = clCreateProgramWithSource(context, 1, (const char **)&program_text, NULL, &err);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Create program from source: ");
		ft_putendl(ocl_error_to_string(err));
		ft_putendl(program_path);
		exit(0);
	}
	build_program(program, device, program_path);
	free(program_text);
	return (program);
}

void				ocl_set_kernel_arg(cl_kernel kernel, cl_uint index, const size_t arg_size, const void *data)
{
	cl_int err;

	err = clSetKernelArg(kernel, index, arg_size, data);
	if(err != CL_SUCCESS)
	{
		ft_putstr("Cannot set kernel argument: ");
		ft_putendl(ocl_error_to_string(err));
		exit(0);
	}
}