cmake_minimum_required(VERSION 3.17)
project(raytracer C)

set(CMAKE_C_STANDARD 90)

INCLUDE_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/include)
INCLUDE_DIRECTORIES(double_linked_list/includes)
INCLUDE_DIRECTORIES(c_linear_algebra/includes)
INCLUDE_DIRECTORIES(libft)

LINK_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/lib)
LINK_DIRECTORIES(libft)
LINK_DIRECTORIES(double_linked_list)
LINK_DIRECTORIES(c_linear_algebra)

add_executable(raytracer camera.c check_timer.c color.c framebuffer.c main.c mainloop.c ray.c render_mask.c scene.c sdl_instance.c shape.c point_light.h)
find_library(OPENCL_UNIT OpenCL)
target_link_libraries(raytracer ${OPENCL_UNIT} SDL2 ft dllist cla)