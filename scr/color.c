#include "color.h"
#include <math.h>

t_vec3 color_color_mul(const t_vec3 * const color1, const t_vec3 * const color2)
{
    return ((t_vec3){{color1->x * color2->x,
                      color1->y * color2->y,
                      color1->z * color2->z}});
}

t_vec3 color_color_sum(const t_vec3 * const color1, const t_vec3 * const color2)
{
    return ((t_vec3){{color1->x + color2->x,
                      color1->y + color2->y,
                      color1->z + color2->z}});
}

void apply_gamma_correction_ptr(t_vec3 * const color, const float exposure, const float gamma)
{
    color->x = powf(color->x * exposure, gamma);
    color->y = powf(color->y * exposure, gamma);
    color->z = powf(color->z * exposure, gamma);
}

t_vec3 apply_gamma_correction(const t_vec3 * const color, const float exposure, const float gamma)
{
    t_vec3 result;

    result.x = powf(color->x * exposure, gamma);
    result.y = powf(color->y * exposure, gamma);
    result.z = powf(color->z * exposure, gamma);
    return (result);
}

t_ivec3 convert_color_to_byte_size(const t_vec3 * const color)
{
    t_ivec3 result;

    result.x = (int)(color->x * 255.0f)%255;
    result.y = (int)(color->y * 255.0f)%255;
    result.z = (int)(color->z * 255.0f)%255;
    return (result);
}
