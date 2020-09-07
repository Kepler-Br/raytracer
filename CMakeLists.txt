cmake_minimum_required(VERSION 3.16)
project(raytracer C)

set(CMAKE_C_STANDARD 99)
SET(CMAKE_BUILD_TYPE Debug)
SET(CL_TARGET_OPENCL_VERSION 120)

INCLUDE_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/include)
INCLUDE_DIRECTORIES(/usr/include)
INCLUDE_DIRECTORIES(headers)
INCLUDE_DIRECTORIES(/usr/include/SDL2/)
INCLUDE_DIRECTORIES(double_linked_list/includes)
INCLUDE_DIRECTORIES(c_linear_algebra/includes)
INCLUDE_DIRECTORIES(libft)

LINK_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/lib)
LINK_DIRECTORIES(libft)
LINK_DIRECTORIES(double_linked_list)
LINK_DIRECTORIES(c_linear_algebra)

add_executable(raytracer src/engine/states/raymarch_state/camera.c src/engine/check_timer.c ./src/color.c src/engine/framebuffer.c ./src/main.c src/engine/mainloop.c ./src/ray.c ./src/render_mask.c src/engine/states/raymarch_state/scene.c src/engine/sdl_instance.c ./src/shape.c ./headers/point_light.h src/engine/input_manager.c src/engine/input_manager.h ./headers/tools.h ./src/tools.c src/ocl_wrapper/ocl_wrapper.c src/ocl_wrapper/ocl_wrapper.h src/ocl_wrapper/ocl_wrapper_get_device.c src/ocl_wrapper/ocl_wrapper_print_info.c src/ocl_wrapper/ocl_create_functions.c src/ocl_wrapper/ocl_program.c src/ocl_wrapper/ocl_work_functions.c src/ocl_wrapper/ocl_buffer.c src/engine/state.c src/engine/state.h src/engine/states/raymarch_state/raytracing_state.c src/engine/states/raymarch_state/raytracing_state.h)
find_library(OPENCL_UNIT OpenCL)
target_link_libraries(raytracer ${OPENCL_UNIT} SDL2 ft dllist cla m)