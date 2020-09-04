#include "linear_alg.h"
#include <math.h>

t_vec2 vec2_vec2_sum(const t_vec2 * const a, const t_vec2 * const b)
{
    return (t_vec2){{a->x + b->x, a->y + b->y}};
}

t_vec2 vec2_vec2_sub(const t_vec2 * const a, const t_vec2 * const b)
{
    return (t_vec2){{a->x - b->x, a->y - b->y}};
}

float vec2_vec2_dot(const t_vec2 * const a, const t_vec2 * const b)
{
    return a->x * b->x + a->y * b->y;
}

float vec2_vec2_cross(const t_vec2 * const a, const t_vec2 * const b)
{
    return a->x * b->y - a->y * b->x;
}

float vec2_len(const t_vec2 * const a)
{
    return sqrtf(a->x * a->x + a->y * a->y);
}

t_vec2 vec2_scalar_mul(const t_vec2 * const a, const float * const b)
{
    return (t_vec2){{a->x * *b, a->y * *b}};
}

t_vec2 vec2_scalar_sum(const t_vec2 * const a, const float * const b)
{
    return (t_vec2){{a->x + *b, a->y + *b}};
}

t_vec2 vec2_scalar_sub(const t_vec2 * const a, const float * const b)
{
    return (t_vec2){{a->x - *b, a->y - *b}};
}

t_vec2 vec2_scalar_div(const t_vec2 * const a, const float * const b)
{
    return (t_vec2){{a->x / *b, a->y / *b}};
}


t_vec2 vec2_clamp(const t_vec2 *const a, float start, float end)
{
    t_vec2 result;

    result = *a;
    vec2_clamp_ptr(&result, start, end);
    return (result);
}

void vec2_clamp_ptr(t_vec2 * const a, float start, float end)
{
    a->x = a->x >= end ? end : a->x;
    a->x = a->x <= start ? start : a->x;

    a->y = a->y >= end ? end : a->y;
    a->y = a->y <= start ? start : a->y;
}





t_vec3 vec3_vec3_sum(const t_vec3 * const a, const t_vec3 * const b)
{
    return (t_vec3){{a->x + b->x, a->y + b->y, a->z + b->z}};
}

t_vec3 vec3_vec3_sub(const t_vec3 * const a, const t_vec3 * const b)
{
    return (t_vec3){{a->x - b->x, a->y - b->y, a->z - b->z}};
}

float vec3_vec3_dot(const t_vec3 * const a, const t_vec3 * const b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

t_vec3 vec3_vec3_cross(const t_vec3 * const a, const t_vec3 * const b)
{
    return (t_vec3){{a->y * b->z - a->z * b->y,
                     a->z * b->x - a->x * b->z,
                     a->x * b->y - a->y * b->x}};
}

t_vec3 vec3_normalize(const t_vec3 * const a)
{
    const float len = vec3_len(a);

    return vec3_scalar_div(a, &len);
}

float vec3_len(const t_vec3 * const a)
{
    return sqrtf(a->x * a->x + a->y * a->y + a->z * a->z);
}

t_vec3 vec3_scalar_mul(const t_vec3 * const a, const float * const b)
{
    return (t_vec3){{a->x * *b, a->y * *b, a->z * *b}};
}

t_vec3 vec3_scalar_sum(const t_vec3 * const a, const float * const b)
{
    return (t_vec3){{a->x + *b, a->y + *b, a->z + *b}};
}

t_vec3 vec3_scalar_sub(const t_vec3 * const a, const float * const b)
{
    return (t_vec3){{a->x - *b, a->y - *b, a->z - *b}};
}

t_vec3 vec3_scalar_div(const t_vec3 * const a, const float * const b)
{
    return (t_vec3){{a->x / *b, a->y / *b, a->z / *b}};
}


t_vec3 vec3_clamp(const t_vec3 *const a, float start, float end)
{
    t_vec3 result;

    result = *a;
    vec3_clamp_ptr(&result, start, end);
    return (result);
}

void vec3_clamp_ptr(t_vec3 * const a, float start, float end)
{
    a->x = a->x >= end ? end : a->x;
    a->x = a->x <= start ? start : a->x;

    a->y = a->y >= end ? end : a->y;
    a->y = a->y <= start ? start : a->y;

    a->z = a->z >= end ? end : a->z;
    a->z = a->z <= start ? start : a->z;
}






t_vec4 vec4_vec4_sum(const t_vec4 * const a, const t_vec4 * const b)
{
    return (t_vec4){{a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w}};
}

t_vec4 vec4_vec4_sub(const t_vec4 * const a, const t_vec4 * const b)
{
    return (t_vec4){{a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w}};
}

float vec4_vec4_dot(const t_vec4 * const a, const t_vec4 * const b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

t_vec4 vec4_normalize(const t_vec4 * const a)
{
    const float len = vec4_len(a);

    return vec4_scalar_div(a, &len);
}

float vec4_len(const t_vec4 * const a)
{
    return sqrtf(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

t_vec4 vec4_scalar_mul(const t_vec4 * const a, const float * const b)
{
    return (t_vec4){{a->x * *b, a->y * *b, a->z * *b, a->w * *b}};
}

t_vec4 vec4_scalar_sum(const t_vec4 * const a, const float * const b)
{
    return (t_vec4){{a->x + *b, a->y + *b, a->z + *b, a->w + *b}};
}

t_vec4 vec4_scalar_sub(const t_vec4 * const a, const float * const b)
{
    return (t_vec4){{a->x - *b, a->y - *b, a->z - *b, a->w - *b}};
}

t_vec4 vec4_scalar_div(const t_vec4 * const a, const float * const b)
{
    return (t_vec4){{a->x / *b, a->y / *b, a->z / *b, a->w / *b}};
}


t_mat4 mat4_mat4_mul(const t_mat4 * const a, const t_mat4 * const b)
{
    t_mat4 r;

    r.s0 = a->s0 * b->s0 + a->s1 * b->s4 + a->s2 * b->s8 + a->s3 * b->sc;
    r.s1 = a->s0 * b->s1 + a->s1 * b->s5 + a->s2 * b->s9 + a->s3 * b->sd;
    r.s2 = a->s0 * b->s2 + a->s1 * b->s6 + a->s2 * b->sa + a->s3 * b->se;
    r.s3 = a->s0 * b->s3 + a->s1 * b->s7 + a->s2 * b->sb + a->s3 * b->sf;
    r.s4 = a->s4 * b->s0 + a->s5 * b->s4 + a->s6 * b->s8 + a->s7 * b->sc;
    r.s5 = a->s4 * b->s1 + a->s5 * b->s5 + a->s6 * b->s9 + a->s7 * b->sd;
    r.s6 = a->s4 * b->s2 + a->s5 * b->s6 + a->s6 * b->sa + a->s7 * b->se;
    r.s7 = a->s4 * b->s3 + a->s5 * b->s7 + a->s6 * b->sb + a->s7 * b->sf;
    r.s8 = a->s8 * b->s0 + a->s9 * b->s4 + a->sa * b->s8 + a->sb * b->sc;
    r.s9 = a->s8 * b->s1 + a->s9 * b->s5 + a->sa * b->s9 + a->sb * b->sd;
    r.sa = a->s8 * b->s2 + a->s9 * b->s6 + a->sa * b->sa + a->sb * b->se;
    r.sb = a->s8 * b->s3 + a->s9 * b->s7 + a->sa * b->sb + a->sb * b->sf;
    r.sc = a->sc * b->s0 + a->sd * b->s4 + a->se * b->s8 + a->sf * b->sc;
    r.sd = a->sc * b->s1 + a->sd * b->s5 + a->se * b->s9 + a->sf * b->sd;
    r.se = a->sc * b->s2 + a->sd * b->s6 + a->se * b->sa + a->sf * b->se;
    r.sf = a->sc * b->s3 + a->sd * b->s7 + a->se * b->sb + a->sf * b->sf;
    return (r);
}

float mat4_det(const t_mat4 * const a)
{
    return (a->s3 * a->s6 * a->s9 * a->sc - a->s2 * a->s7 * a->s9 * a->sc-
            a->s3 * a->s5 * a->sa * a->sc+a->s1 * a->s7 * a->sa * a->sc+
            a->s2 * a->s5 * a->sb * a->sc-a->s1 * a->s6 * a->sb * a->sc-
            a->s3 * a->s6 * a->s8 * a->sd+a->s2 * a->s7 * a->s8 * a->sd+
            a->s3 * a->s4 * a->sa * a->sd-a->s0 * a->s7 * a->sa * a->sd-
            a->s2 * a->s4 * a->sb * a->sd+a->s0 * a->s6 * a->sb * a->sd+
            a->s3 * a->s5 * a->s8 * a->se-a->s1 * a->s7 * a->s8 * a->se-
            a->s3 * a->s4 * a->s9 * a->se+a->s0 * a->s7 * a->s9 * a->se+
            a->s1 * a->s4 * a->sb * a->se-a->s0 * a->s5 * a->sb * a->se-
            a->s2 * a->s5 * a->s8 * a->sf+a->s1 * a->s6 * a->s8 * a->sf+
            a->s2 * a->s4 * a->s9 * a->sf-a->s0 * a->s6 * a->s9 * a->sf-
            a->s1 * a->s4 * a->sa * a->sf+a->s0 * a->s5 * a->sa * a->sf);
}

t_mat4 mat4_inverse(const t_mat4 * const a)
{
    t_mat4 inv;
    float det;
    int i;
    inv.s[0] = a->s[5]  * a->s[10] * a->s[15] -
             a->s[5]  * a->s[11] * a->s[14] -
             a->s[9]  * a->s[6]  * a->s[15] +
             a->s[9]  * a->s[7]  * a->s[14] +
             a->s[13] * a->s[6]  * a->s[11] -
             a->s[13] * a->s[7]  * a->s[10];

    inv.s[4] = -a->s[4]  * a->s[10] * a->s[15] +
             a->s[4]  * a->s[11] * a->s[14] +
             a->s[8]  * a->s[6]  * a->s[15] -
             a->s[8]  * a->s[7]  * a->s[14] -
             a->s[12] * a->s[6]  * a->s[11] +
             a->s[12] * a->s[7]  * a->s[10];

    inv.s[8] = a->s[4]  * a->s[9] * a->s[15] -
             a->s[4]  * a->s[11] * a->s[13] -
             a->s[8]  * a->s[5] * a->s[15] +
             a->s[8]  * a->s[7] * a->s[13] +
             a->s[12] * a->s[5] * a->s[11] -
             a->s[12] * a->s[7] * a->s[9];

    inv.s[12] = -a->s[4]  * a->s[9] * a->s[14] +
              a->s[4]  * a->s[10] * a->s[13] +
              a->s[8]  * a->s[5] * a->s[14] -
              a->s[8]  * a->s[6] * a->s[13] -
              a->s[12] * a->s[5] * a->s[10] +
              a->s[12] * a->s[6] * a->s[9];

    inv.s[1] = -a->s[1]  * a->s[10] * a->s[15] +
             a->s[1]  * a->s[11] * a->s[14] +
             a->s[9]  * a->s[2] * a->s[15] -
             a->s[9]  * a->s[3] * a->s[14] -
             a->s[13] * a->s[2] * a->s[11] +
             a->s[13] * a->s[3] * a->s[10];

    inv.s[5] = a->s[0]  * a->s[10] * a->s[15] -
             a->s[0]  * a->s[11] * a->s[14] -
             a->s[8]  * a->s[2] * a->s[15] +
             a->s[8]  * a->s[3] * a->s[14] +
             a->s[12] * a->s[2] * a->s[11] -
             a->s[12] * a->s[3] * a->s[10];

    inv.s[9] = -a->s[0]  * a->s[9] * a->s[15] +
             a->s[0]  * a->s[11] * a->s[13] +
             a->s[8]  * a->s[1] * a->s[15] -
             a->s[8]  * a->s[3] * a->s[13] -
             a->s[12] * a->s[1] * a->s[11] +
             a->s[12] * a->s[3] * a->s[9];

    inv.s[13] = a->s[0]  * a->s[9] * a->s[14] -
              a->s[0]  * a->s[10] * a->s[13] -
              a->s[8]  * a->s[1] * a->s[14] +
              a->s[8]  * a->s[2] * a->s[13] +
              a->s[12] * a->s[1] * a->s[10] -
              a->s[12] * a->s[2] * a->s[9];

    inv.s[2] = a->s[1]  * a->s[6] * a->s[15] -
             a->s[1]  * a->s[7] * a->s[14] -
             a->s[5]  * a->s[2] * a->s[15] +
             a->s[5]  * a->s[3] * a->s[14] +
             a->s[13] * a->s[2] * a->s[7] -
             a->s[13] * a->s[3] * a->s[6];

    inv.s[6] = -a->s[0]  * a->s[6] * a->s[15] +
             a->s[0]  * a->s[7] * a->s[14] +
             a->s[4]  * a->s[2] * a->s[15] -
             a->s[4]  * a->s[3] * a->s[14] -
             a->s[12] * a->s[2] * a->s[7] +
             a->s[12] * a->s[3] * a->s[6];

    inv.s[10] = a->s[0]  * a->s[5] * a->s[15] -
              a->s[0]  * a->s[7] * a->s[13] -
              a->s[4]  * a->s[1] * a->s[15] +
              a->s[4]  * a->s[3] * a->s[13] +
              a->s[12] * a->s[1] * a->s[7] -
              a->s[12] * a->s[3] * a->s[5];

    inv.s[14] = -a->s[0]  * a->s[5] * a->s[14] +
              a->s[0]  * a->s[6] * a->s[13] +
              a->s[4]  * a->s[1] * a->s[14] -
              a->s[4]  * a->s[2] * a->s[13] -
              a->s[12] * a->s[1] * a->s[6] +
              a->s[12] * a->s[2] * a->s[5];

    inv.s[3] = -a->s[1] * a->s[6] * a->s[11] +
             a->s[1] * a->s[7] * a->s[10] +
             a->s[5] * a->s[2] * a->s[11] -
             a->s[5] * a->s[3] * a->s[10] -
             a->s[9] * a->s[2] * a->s[7] +
             a->s[9] * a->s[3] * a->s[6];

    inv.s[7] = a->s[0] * a->s[6] * a->s[11] -
             a->s[0] * a->s[7] * a->s[10] -
             a->s[4] * a->s[2] * a->s[11] +
             a->s[4] * a->s[3] * a->s[10] +
             a->s[8] * a->s[2] * a->s[7] -
             a->s[8] * a->s[3] * a->s[6];

    inv.s[11] = -a->s[0] * a->s[5] * a->s[11] +
              a->s[0] * a->s[7] * a->s[9] +
              a->s[4] * a->s[1] * a->s[11] -
              a->s[4] * a->s[3] * a->s[9] -
              a->s[8] * a->s[1] * a->s[7] +
              a->s[8] * a->s[3] * a->s[5];

    inv.s[15] = a->s[0] * a->s[5] * a->s[10] -
              a->s[0] * a->s[6] * a->s[9] -
              a->s[4] * a->s[1] * a->s[10] +
              a->s[4] * a->s[2] * a->s[9] +
              a->s[8] * a->s[1] * a->s[6] -
              a->s[8] * a->s[2] * a->s[5];

    det = a->s[0] * inv.s[0] + a->s[1] * inv.s[4] + a->s[2] * inv.s[8] + a->s[3] * inv.s[12];
    if (det == 0)
    {
        return (*a);
    }
    det = 1.0f / det;
    i = 0;
    while (i < 16)
    {
        inv.s[i] = inv.s[i] * det;
        i++;
    }
    return (inv);
}
t_mat4 mat4_scalar_mul(const t_mat4 * const a, const float * const b)
{
    static const size_t total_components = 16;
    size_t i;
    t_mat4 new_mat;

    i = 0;
    while(i < total_components)
    {
        new_mat.s[i] = a->s[i] * *b;
        i++;
    }
    return (new_mat);
}

t_mat4 mat4_scalar_sum(const t_mat4 * const a, const float * const b)
{
    static const size_t total_components = 16;
    size_t i;
    t_mat4 new_mat;

    i = 0;
    while(i < total_components)
    {
        new_mat.s[i] = a->s[i] + *b;
        i++;
    }
    return (new_mat);
}

t_mat4 mat4_scalar_sub(const t_mat4 * const a, const float * const b)
{
    static const size_t total_components = 16;
    size_t i;
    t_mat4 new_mat;

    i = 0;
    while(i < total_components)
    {
        new_mat.s[i] = a->s[i] - *b;
        i++;
    }
    return (new_mat);
}

t_mat4 mat4_scalar_div(const t_mat4 * const a, const float * const b)
{
    static const size_t total_components = 16;
    size_t i;
    t_mat4 new_mat;

    i = 0;
    while(i < total_components)
    {
        new_mat.s[i] = a->s[i] / *b;
        i++;
    }
    return (new_mat);
}

t_ivec2 ivec2_ivec2_sum(const t_ivec2 * const a, const t_ivec2 * const b)
{
    return (t_ivec2){{a->x + b->x, a->y + b->y}};
}

t_ivec2 ivec2_scalar_sub(const t_ivec2 * const a, const float * const b)
{
    return (t_ivec2){{a->x - *b, a->y - *b}};
}

t_ivec2 ivec2_scalar_sum(const t_ivec2 * const a, const float * const b)
{
    return (t_ivec2){{a->x + *b, a->y + *b}};
}

t_ivec2 ivec2_ivec2_sub(const t_ivec2 * const a, const t_ivec2 * const b)
{
    return (t_ivec2){{a->x - b->x, a->y - b->y}};
}

t_ivec2 ivec2_scalar_mul(const t_ivec2 * const a, const float * const b)
{
    return (t_ivec2){{a->x * *b, a->y * *b}};
}

t_ivec2 ivec2_scalar_div(const t_ivec2 * const a, const float * const b)
{
    return (t_ivec2){{a->x / *b, a->y / *b}};
}

void ivec2_ivec2_sum_ptr(t_ivec2 * const a, const t_ivec2 * const b)
{
    a->x += b->x;
    a->y += b->y;
}

void ivec2_ivec2_sub_ptr(t_ivec2 * const a, const t_ivec2 * const b)
{
    a->x -= b->x;
    a->y -= b->y;
}

void ivec2_scalar_sum_ptr(t_ivec2 * const a, const float * const b)
{
    a->x += *b;
    a->y += *b;
}

void ivec2_scalar_sub_ptr(t_ivec2 * const a, const float * const b)
{
    a->x -= *b;
    a->y -= *b;
}

void ivec2_scalar_mul_ptr(t_ivec2 * const a, const float * const b)
{
    a->x *= *b;
    a->y *= *b;
}

void ivec2_scalar_div_ptr(t_ivec2 * const a, const float * const b)
{
    a->x /= *b;
    a->y /= *b;
}
