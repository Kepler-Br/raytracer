#include "ocl_wrapper.h"

static cl_device_id get_device_max_fequency_from_platform(cl_platform_id platform, cl_uint *max_freq)
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

static t_ocl_device get_device_max_frequency()
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

static cl_device_id get_device_max_compute_units_from_platform(cl_platform_id platform, cl_uint *max_compute_units)
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

static t_ocl_device get_device_max_compute_units()
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

cl_device_id		ocl_get_device(const int type)
{
	if(type == OCL_WRAPPER_MAX_COMPUTE_UNITS)
		return (get_device_max_compute_units().device_id);
	else if(type == OCL_WRAPPER_MAX_FREQ)
		return (get_device_max_frequency().device_id);
	return (get_device_max_frequency().device_id);
}