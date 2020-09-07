#include <stdio.h>
#include "engine/mainloop.h"

#include "ocl_wrapper/ocl_wrapper.h"

#include <stdio.h>
#include "engine/mainloop.h"

#include "ocl_wrapper/ocl_wrapper.h"
#include "./engine/state.h"
#include "./engine/states/raymarch_state/raytracing_state.h"

void print_int_array(cl_int *array, size_t size)
{
	cl_uint i = 0;
	while (i < size)
	{
		printf("%u ",array[i]);
		i++;
	}
	printf("\n");
}

int main()
{
//	opencl_info();
//	t_ocl_device best = get_device_max_compute_units();
//	print_platform_info(best.platform_id);
//	print_device_info_full(best.device_id);

//	cl_context context;
//	cl_command_queue commands;
//	cl_program program;
//	cl_kernel main_kernel;
//	cl_device_id device_id;
//	cl_mem mem_array_obj;
//
//	device_id = ocl_get_device(OCL_WRAPPER_MAX_COMPUTE_UNITS);
//	context = ocl_create_context(device_id);
//	commands = ocl_create_command_queue(context, device_id);
//	program = ocl_load_and_build_program(context, device_id, "./OpenCLPrograms/helloworld.cl");
//	main_kernel = ocl_get_kernel(program, "main_kernel");
//
//
//	const size_t array_length = 80*60;
//	cl_int *array_to_change = malloc(sizeof(cl_int)*array_length);
//	print_int_array(array_to_change, array_length);
//	mem_array_obj = ocl_create_buffer(context, CL_MEM_WRITE_ONLY, array_length * sizeof(cl_int), NULL);
//	ocl_set_kernel_arg(main_kernel, 0, sizeof(cl_mem), (void*)&mem_array_obj);
//	ocl_enqueue_write_buffer(commands, mem_array_obj, array_length * sizeof(cl_int), (void *)array_to_change);
//	size_t local_work_size = 1;
//	ocl_enqueue_nd_range_kernel(commands, main_kernel, 1, &array_length, &local_work_size);
//	ocl_finish(commands);
//	ocl_enqueue_read_buffer(commands, mem_array_obj, array_length * sizeof(cl_int), (void *)array_to_change);
//	print_int_array(array_to_change, array_length);
//
//	clReleaseMemObject(mem_array_obj);
//	clReleaseKernel(main_kernel);
//	clReleaseProgram(program);
//	clReleaseDevice(device_id);
//	clReleaseCommandQueue(commands);
//	clReleaseContext(context);



    t_mainloop *mainloop;
    t_state *raytracing_state;
	t_ivec2 resolution = (t_ivec2){{800, 600}};

    mainloop = construct_mainloop(resolution, "Realtime raytrasing");
	raytracing_state = construct_raytracing_state(mainloop->input_manager, mainloop->sdl_instance, mainloop);
	mainloop->push_state(mainloop, raytracing_state);
    mainloop->run(mainloop);
	destruct_mainloop(mainloop);
	free(mainloop);
	return 0;
}
