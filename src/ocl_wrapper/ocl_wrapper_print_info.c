#include "ocl_wrapper.h"
#include <stdio.h>

static void print_device_info_string(cl_device_id device, cl_bitfield type, char *what_to_print)
{
	char *line;
	size_t line_size;

	clGetDeviceInfo(device, type, 0, NULL, &line_size);
	line = (char*)malloc(sizeof(char)*line_size);
	clGetDeviceInfo(device, type, line_size, line, NULL);
	printf("%s: %s\n", what_to_print, line);
	free(line);
}

void				ocl_print_all_information()
{
	cl_uint total_platforms;
	cl_platform_id *platform_ids;
	cl_uint i;
	cl_uint total_devices;
	cl_device_id *device_ids;

	clGetPlatformIDs(0, 0, &total_platforms);
	if(total_platforms == 0)
	{
		printf("No platforms found.\n");
		return;
	}
	printf("Total platforms: %u\n", total_platforms);

	platform_ids = malloc(sizeof(cl_platform_id)*total_platforms);
	clGetPlatformIDs(total_platforms, platform_ids, 0);

	i = 0;
	printf("Platforms: \n----------\n");
	while(i < total_platforms)
	{
		ocl_print_platform_info(platform_ids[i]);
		i++;
		printf("----------\n");
	}
	printf("\n");

	clGetDeviceIDs( platform_ids[0], CL_DEVICE_TYPE_ALL, NULL, NULL, &total_devices);
	if(total_devices == 0)
	{
		printf("No devices found.\n");
		return;
	}
	printf("Total devices: %u\n", total_devices);
	device_ids = malloc(sizeof(cl_device_id)*total_devices);
	clGetDeviceIDs(platform_ids[0], CL_DEVICE_TYPE_ALL, total_devices, device_ids, NULL);
	i = 0;
	printf("Devices: \n----------\n");
	while(i < total_devices)
	{
		ocl_print_device_info_full(device_ids[i]);
		i++;
		printf("----------\n");
	}
}

static void 		private_ocl_print_work_dimention_groups(cl_device_id device)
{
	size_t *work_groups;
	cl_uint dimensions;
	size_t index;

	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &dimensions, NULL);
	work_groups = malloc(sizeof(size_t)*dimensions);
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*dimensions, work_groups, NULL);
	index = 0;
	printf("Maximum work item sizes: [ ");
	while(index < dimensions)
	{
		printf("%lu ", work_groups[index]);
		index++;
	}
	printf("]\n");
}

void				ocl_print_device_info_full(cl_device_id device)
{
	size_t num;
	cl_uint num2;
	cl_ulong num3;


	print_device_info_string(device, CL_DEVICE_NAME, "Device name");
	clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &num2, NULL);
	printf("Max compute units: %u\n", num2);
	clGetDeviceInfo(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &num2, NULL);
	printf("Max clock frequency: %u MHz(%u.%u GHz)\n", num2, num2/1000, num2%1000);
	clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &num, NULL);
	printf("Max work group size: %lu\n", num);
	clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_ulong), &num3, NULL);
	private_ocl_print_work_dimention_groups(device);
	printf("Device type: ");
	if(num3 == CL_DEVICE_TYPE_CPU)
		printf("CPU");
	else if (num3 == CL_DEVICE_TYPE_GPU)
		printf("GPU");
	else
		printf("Another");
	printf("\n");
}

void				ocl_print_platform_info(cl_platform_id platform)
{
	char *line;
	const size_t line_size = 5000;

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
