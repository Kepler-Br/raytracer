#ifndef TOOLS_H
#define TOOLS_H

int randi(t_random *random)
{
    uint seed = random->global_id.x * random->global_id.y + random->host_random_number + random->iteration;
    uint t = seed ^ (seed << 11);  
    uint result = random->global_id.y ^ (random->global_id.y >> 19) ^ (t ^ (t >> 8));
    random->iteration++;
    return (random->array[result%random->size]);
}

float randf(t_random *random)
{
    uint seed = random->global_id.x * random->global_id.y + random->host_random_number + random->iteration;
    uint t = seed ^ (seed << 11);  
    uint result = random->global_id.y ^ (random->global_id.y >> 19) ^ (t ^ (t >> 8));
    random->iteration++;
    return ((float)(random->array[result%random->size]%9999999999)/9999999999);
}

float3 rand_point_on_hemisphere(t_random *random)
{
    // float3 result;
    // float theta;
    // float phi;
    
    // theta = randf(random)*2.0f*M_PI_F;
    // phi = randf(random)*(M_PI_F/2.0f);

    // result = (float3){
    //     sin(theta) * cos(phi),
    //     cos(theta),
    //     sin(theta) * sin(phi)
    // };
    // return (result);
    float r1 = randf(random)*2.0f*M_PI_F;
    float r2 = randf(random)*(M_PI_F/2.0f);
    float sinTheta = sqrt(1 - r1 * r1); 
    float phi = 2 * M_PI_F * r2; 
    float x = sinTheta * cos(phi); 
    float z = sinTheta * sin(phi);
    return (float3){x, r1, z}; 
}


void coordinate_system_from_normal(float3 n, float3 *nt, float3 *nb)
{
    if (fabs(n.x) > fabs(n.y)) 
        *nt = (float3){n.z, 0, -n.x} / sqrt(n.x * n.x + n.z * n.z); 
    else 
        *nt = (float3){0, -n.z, n.y} / sqrt(n.y * n.y + n.z * n.z); 
    *nb = cross(n, *nt); 
}

float3 to_world_coordinates(float3 normal, float3 sample)
{
    float3 nt;
    float3 nb;
    coordinate_system_from_normal(normal, &nt, &nb);
    return (float3){
         sample.x * nb.x + sample.y * normal.x + sample.z * nt.x, 
        sample.x * nb.y + sample.y * normal.y + sample.z * nt.y, 
        sample.x * nb.z + sample.y * normal.z + sample.z * nt.z
    };
}

#endif