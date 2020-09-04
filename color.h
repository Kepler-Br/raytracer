#ifndef COLOR_H
#define COLOR_H

#include "linear_alg.h"

t_vec3 color_color_mul(const t_vec3 * const color1, const t_vec3 * const color2);
t_vec3 color_color_sum(const t_vec3 * const color1, const t_vec3 * const color2);
void apply_gamma_correction_ptr(t_vec3 *const color, const float exposure, const float gamma);
t_vec3 apply_gamma_correction(const t_vec3 *const color, const float exposure, const float gamma);
t_ivec3 convert_color_to_byte_size(const t_vec3 *const color);

#endif // COLOR_H
