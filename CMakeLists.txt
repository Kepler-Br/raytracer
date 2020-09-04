cmake_minimum_required(VERSION 3.17)
project(raytracer C)

set(CMAKE_C_STANDARD 90)

INCLUDE_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/include)
INCLUDE_DIRECTORIES(double_linked_list/includes)
INCLUDE_DIRECTORIES(libft)

LINK_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/lib)
LINK_DIRECTORIES(libft)

find_package(double_linked_list CONFIG)

add_executable(raytracer camera.c check_timer.c color.c framebuffer.c linear_alg.c main.c mainloop.c ray.c render_mask.c scene.c sdl_instance.c shape.c)
find_library(OPENCL_UNIT OpenCL)
target_link_libraries(raytracer ${OPENCL_UNIT} SDL2 ft double_linked_list)