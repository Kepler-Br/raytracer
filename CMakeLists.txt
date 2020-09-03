cmake_minimum_required(VERSION 3.17)
project(raytracer C)

set(CMAKE_C_STANDARD 99)

INCLUDE_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/include)
LINK_DIRECTORIES(~/.brew/Cellar/sdl2/2.0.12_1/lib)
add_executable(raytracer check_timer.c framebuffer.c linear_alg.c main.c mainloop.c sdl_instance.c)
find_library(OPENCL_UNIT OpenCL)
target_link_libraries(raytracer ${OPENCL_UNIT} SDL2)