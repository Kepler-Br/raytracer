#include <stdio.h>
#include "linear_alg.h"

int main()
{
    t_vec2 vec = (t_vec2){{10.0f, 9.0f}};
    t_vec2 vec2 = (t_vec2){{2.0f, 3.0f}};
    t_vec2 vec3 = vec2_vec2_sub(&vec, &vec2);
    printf("%f, %f\n", vec3.x, vec3.y);
    return 0;
}
