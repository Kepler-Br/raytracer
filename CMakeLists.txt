cmake_minimum_required(VERSION 3.16)
project(raytracer C)

set(CMAKE_C_STANDARD 90)

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

add_executable(raytracer ./src/camera.c ./src/check_timer.c ./src/color.c ./src/framebuffer.c ./src/main.c ./src/mainloop.c ./src/ray.c ./src/render_mask.c ./src/scene.c ./src/sdl_instance.c ./src/shape.c ./headers/point_light.h ./src/input_manager.c ./headers/input_manager.h ./headers/tools.h ./src/tools.c)
find_library(OPENCL_UNIT OpenCL)
target_link_libraries(raytracer ${OPENCL_UNIT} SDL2 ft dllist cla m)