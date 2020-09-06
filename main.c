#include <stdio.h>
#include "mainloop.h"

void print_platform_info(cl_platform_id platform)
{
	char *line;
	const size_t line_size = 1000;

	line = malloc(sizeof(char)*line_size);
	clGetPlatformInfo(platform, CL_PLATFORM_PROFILE, line_size, line, NULL);
	printf("Profile: %s\n", line);
	clGetPlatformInfo(platform, CL_PLATFORM_VERSION, line_size, line, NULL);
	printf("Version: %s\n", line);
	clGetPlatformInfo(platform, CL_PLATFORM_NAME, line_size, line, NULL);
	printf("Name: %s\n", line);
	clGetPlatformInfo(platform, CL_PLATFORM_VENDOR, line_size, line, NULL);
	printf("Vendor: %s\n", line);
	free(line);
}

void print_device_info_string(cl_device_id device, uint type, char *what_to_print)
{
	char *line;
	size_t line_size;

	clGetDeviceInfo(device, type, 0, NULL, &line_size);
	line = (char*)malloc(sizeof(char)*line_size);
	clGetDeviceInfo(device, type, line_size, line, NULL);
	printf("%s: %s\n", what_to_print, line);
	free(line);
}

void print_device_info_full(cl_device_id device)
{
	cl_uint num2;
	cl_ulong num3;

	print_device_info_string(device, CL_DEVICE_NAME, "Device name");
	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num2, NULL);
	printf("Max compute units: %u\n", num2);
	clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &num2, NULL);
	printf("Max clock frequency: %u MHz(%u.%u GHz)\n", num2, num2/1000, num2%1000);
	clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_ulong), &num3, NULL);
	printf("Device type: ");
	if(num3 == CL_DEVICE_TYPE_CPU)
		printf("CPU");
	else if (num3 == CL_DEVICE_TYPE_GPU)
		printf("GPU");
	else
		printf("Another");
	printf("\n");
}

void opencl_info()
{
	cl_uint total_platforms;
	cl_platform_id *platform_ids;
	cl_uint i;
	cl_uint total_devices;
	cl_device_id *device_ids;

	clGetPlatformIDs(0, 0, &total_platforms);
	printf("Total platforms: %u\n", total_platforms);

	platform_ids = malloc(sizeof(cl_platform_id)*total_platforms);
	clGetPlatformIDs(total_platforms, platform_ids, 0);

	i = 0;
	printf("Platforms: \n----------\n");
	while(i < total_platforms)
	{
		print_platform_info(platform_ids[i]);
		i++;
		printf("----------\n");
	}
	printf("\n");

	clGetDeviceIDs( platform_ids[0], CL_DEVICE_TYPE_ALL, NULL, NULL, &total_devices);
	printf("Total devices: %u\n", total_devices);
	device_ids = malloc(sizeof(cl_device_id)*total_devices);
	clGetDeviceIDs(platform_ids[0], CL_DEVICE_TYPE_ALL, total_devices, device_ids, NULL);
	i = 0;
	printf("Devices: \n----------\n");
	while(i < total_devices)
	{
		print_device_info_full(device_ids[i]);
		i++;
		printf("----------\n");
	}
}

typedef struct s_ocl_device
{
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_uint freq;
	cl_uint compute_units;
} t_ocl_device;

cl_device_id get_device_max_fequency_from_platform(cl_platform_id platform, cl_uint *max_freq)
{
	cl_uint i;
	cl_uint total_devices;
	cl_device_id *device_ids;
	cl_uint freq;
	cl_device_id best_device;

	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, NULL, NULL, &total_devices);
	device_ids = malloc(sizeof(cl_device_id)*total_devices);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, total_devices, device_ids, NULL);
	i = 0;
	while(i < total_devices)
	{
		clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &freq, NULL);
		if(*max_freq == 0 || *max_freq < freq)
		{
			best_device = device_ids[i];
			*max_freq = freq;
		}
		i++;
	}
	free(device_ids);
	return (best_device);
}

t_ocl_device get_device_max_frequency()
{
	cl_uint total_platforms;
	cl_platform_id *platform_ids;
	cl_uint i;
	t_ocl_device cur;
	t_ocl_device best;

	clGetPlatformIDs(0, 0, &total_platforms);
	platform_ids = malloc(sizeof(cl_platform_id)*total_platforms);
	clGetPlatformIDs(total_platforms, platform_ids, 0);
	i = 0;
	cur.freq = 0;
	while (i < total_platforms)
	{
		best.platform_id = platform_ids[i];
		best.freq = 0;
		best.device_id = get_device_max_fequency_from_platform(platform_ids[i], &best.freq);
		if(cur.freq == 0 || cur.freq < best.freq)
			cur = best;
		i++;
	}
	free(platform_ids);
	return (best);
}

cl_device_id get_device_max_compute_units_from_platform(cl_platform_id platform, cl_uint *max_compute_units)
{
	cl_uint i;
	cl_uint total_devices;
	cl_device_id *device_ids;
	cl_uint compute_units;
	cl_device_id best_device;

	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, NULL, NULL, &total_devices);
	device_ids = malloc(sizeof(cl_device_id)*total_devices);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, total_devices, device_ids, NULL);
	i = 0;
	while(i < total_devices)
	{
		clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &compute_units, NULL);
		if(*max_compute_units == 0 || *max_compute_units < compute_units)
		{
			best_device = device_ids[i];
			*max_compute_units = compute_units;
		}
		i++;
	}
	free(device_ids);
	return (best_device);
}

t_ocl_device get_device_max_compute_units()
{
	cl_uint total_platforms;
	cl_platform_id *platform_ids;
	cl_uint i;
	t_ocl_device cur;
	t_ocl_device best;

	clGetPlatformIDs(0, 0, &total_platforms);
	platform_ids = malloc(sizeof(cl_platform_id)*total_platforms);
	clGetPlatformIDs(total_platforms, platform_ids, 0);
	i = 0;
	cur.compute_units = 0;
	while (i < total_platforms)
	{
		best.platform_id = platform_ids[i];
		best.compute_units = 0;
		best.device_id = get_device_max_compute_units_from_platform(platform_ids[i], &best.compute_units);
		if(cur.compute_units == 0 || cur.compute_units < best.compute_units)
			cur = best;
		i++;
	}
	free(platform_ids);
	return (best);
}

#include "tools.h"

int main()
{
//	opencl_info();
	t_ocl_device best = get_device_max_compute_units();
	print_platform_info(best.platform_id);
	print_device_info_full(best.device_id);

	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel main_kernel;
	cl_int err;

	context = clCreateContext(0, 1, &best.device_id, NULL, NULL, &err);
	commands = clCreateCommandQueue(context, best.device_id, 0, &err);
	char *program_text = read_file("./OpenCLPrograms/helloworld.cl");
	program = clCreateProgramWithSource(context, 1, (const char **)&program_text, NULL, &err);
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	if (err != CL_SUCCESS) {
		char buf[2048];
		std::cout << "Error: Failed to build program" << std::endl;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buf), buf, &length);
		std::cout << buf << std::endl;
		std::getchar();
		running = false;
		exit(1);
	}
//    t_mainloop *mainloop;
//    mainloop = construct_mainloop((t_ivec2){{800, 600}}, "Realtime raytrasing");
//
//    mainloop->run(mainloop);
    return 0;
}
