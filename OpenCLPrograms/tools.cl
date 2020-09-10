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
    float3 result;
    float theta;
    float phi;
    
    theta = randf(random)*2.0f*M_PI_F;
    phi = randf(random)*(M_PI_F/2.0f);

    result = (float3){
        sin(theta) * cos(phi),
        cos(theta),
        sin(theta) * sin(phi)
    };
    return (result);
}

#endif