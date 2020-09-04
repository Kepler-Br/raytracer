#ifndef VEC_H
#define VEC_H

#ifdef __APPLE__
# include <OpenCL/cl.h>
#elif __linux__
# include <CL/cl.h>
#endif


typedef cl_float2 t_vec2;
typedef cl_float3 t_vec3;
typedef cl_float4 t_vec4;
typedef cl_int2 t_ivec2;
typedef cl_int3 t_ivec3;
typedef cl_int4 t_ivec4;
typedef cl_float16 t_mat4;

t_ivec2 ivec2_ivec2_sum(const t_ivec2 * const a, const t_ivec2 * const b);
t_ivec2 ivec2_ivec2_sub(const t_ivec2 * const a, const t_ivec2 * const b);
t_ivec2 ivec2_scalar_sum(const t_ivec2 * const a, const float * const b);
t_ivec2 ivec2_scalar_sub(const t_ivec2 * const a, const float * const b);
t_ivec2 ivec2_scalar_mul(const t_ivec2 * const a, const float * const b);
t_ivec2 ivec2_scalar_div(const t_ivec2 * const a, const float * const b);

void ivec2_ivec2_sum_ptr(t_ivec2 * const a, const t_ivec2 * const b);
void ivec2_ivec2_sub_ptr(t_ivec2 * const a, const t_ivec2 * const b);
void ivec2_scalar_sum_ptr(t_ivec2 * const a, const float * const b);
void ivec2_scalar_sub_ptr(t_ivec2 * const a, const float * const b);
void ivec2_scalar_mul_ptr(t_ivec2 * const a, const float * const b);
void ivec2_scalar_div_ptr(t_ivec2 * const a, const float * const b);

t_vec2 vec2_vec2_sum(const t_vec2 * const a, const t_vec2 * const b);
t_vec2 vec2_vec2_sub(const t_vec2 * const a, const t_vec2 * const b);
float vec2_vec2_dot(const t_vec2 * const a, const t_vec2 * const b);
float vec2_vec2_cross(const t_vec2 * const a, const t_vec2 * const b);
t_vec2 vec2_normalize(const t_vec3 * const a);
float vec2_len(const t_vec2 * const a);
t_vec2 vec2_scalar_mul(const t_vec2 * const a, const float * const b);
t_vec2 vec2_scalar_sum(const t_vec2 * const a, const float * const b);
t_vec2 vec2_scalar_sub(const t_vec2 * const a, const float * const b);
t_vec2 vec2_scalar_div(const t_vec2 * const a, const float * const b);
t_vec2 vec2_clamp(const t_vec2 *const a, float start, float end);
void vec2_clamp_ptr(t_vec2 * const a, float start, float end);


t_vec3 vec3_vec3_sum(const t_vec3 * const a, const t_vec3 * const b);
t_vec3 vec3_vec3_sub(const t_vec3 * const a, const t_vec3 * const b);
float vec3_vec3_dot(const t_vec3 * const a, const t_vec3 * const b);
t_vec3 vec3_vec3_cross(const t_vec3 * const a, const t_vec3 * const b);
t_vec3 vec3_normalize(const t_vec3 * const a);
float vec3_len(const t_vec3 * const a);
t_vec3 vec3_scalar_mul(const t_vec3 * const a, const float * const b);
t_vec3 vec3_scalar_sum(const t_vec3 * const a, const float * const b);
t_vec3 vec3_scalar_sub(const t_vec3 * const a, const float * const b);
t_vec3 vec3_scalar_div(const t_vec3 * const a, const float * const b);
t_vec3 vec3_clamp(const t_vec3 *const a, float start, float end);
void vec3_clamp_ptr(t_vec3 * const a, float start, float end);


t_vec4 vec4_vec4_sum(const t_vec4 * const a, const t_vec4 * const b);
t_vec4 vec4_vec4_sub(const t_vec4 * const a, const t_vec4 * const b);
float vec4_vec4_dot(const t_vec4 * const a, const t_vec4 * const b);
t_vec4 vec4_normalize(const t_vec4 * const a);
float vec4_len(const t_vec4 * const a);
t_vec4 vec4_scalar_mul(const t_vec4 * const a, const float * const b);
t_vec4 vec4_scalar_sum(const t_vec4 * const a, const float * const b);
t_vec4 vec4_scalar_sub(const t_vec4 * const a, const float * const b);
t_vec4 vec4_scalar_div(const t_vec4 * const a, const float * const b);

t_mat4 mat4_mat4_mul(const t_mat4 * const a, const t_mat4 * const b);
float mat4_det(const t_mat4 * const a);
t_mat4 mat4_inverse(const t_mat4 * const a);
t_mat4 mat4_scalar_mul(const t_mat4 * const a, const float * const b);
t_mat4 mat4_scalar_sum(const t_mat4 * const a, const float * const b);
t_mat4 mat4_scalar_sub(const t_mat4 * const a, const float * const b);
t_mat4 mat4_scalar_div(const t_mat4 * const a, const float * const b);

#endif // VEC_H
