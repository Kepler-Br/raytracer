#ifndef TOOLS_H
#define TOOLS_H

#include "tools.cl"

enum{ MWC64X_A = 4294883355U };
enum{ MWC64X_M = 18446383549859758079UL };



void MWC64X_Step(mwc64x_state_t *s)
{
	uint X=s->x, C=s->c;
	
	uint Xn=MWC64X_A*X+C;
	uint carry=(uint)(Xn<C);				// The (Xn<C) will be zero or one for scalar
	uint Cn=mad_hi(MWC64X_A,X,carry);  
	
	s->x=Xn;
	s->c=Cn;
}

uint MWC64X_NextUint(mwc64x_state_t *s)
{
	uint res=s->x ^ s->c;
	MWC64X_Step(s);
	return res;
}

// Pre: a<M, b<M
// Post: r=(a+b) mod M
ulong MWC_AddMod64(ulong a, ulong b, ulong M)
{
	ulong v=a+b;
	if( (v>=M) || (v<a) )
		v=v-M;
	return v;
}

// Pre: a<M,b<M
// Post: r=(a*b) mod M
// This could be done more efficently, but it is portable, and should
// be easy to understand. It can be replaced with any of the better
// modular multiplication algorithms (for example if you know you have
// double precision available or something).
ulong MWC_MulMod64(ulong a, ulong b, ulong M)
{	
	ulong r=0;
	while(a!=0){
		if(a&1)
			r=MWC_AddMod64(r,b,M);
		b=MWC_AddMod64(b,b,M);
		a=a>>1;
	}
	return r;
}


// Pre: a<M, e>=0
// Post: r=(a^b) mod M
// This takes at most ~64^2 modular additions, so probably about 2^15 or so instructions on
// most architectures
ulong MWC_PowMod64(ulong a, ulong e, ulong M)
{
	ulong sqr=a, acc=1;
	while(e!=0){
		if(e&1)
			acc=MWC_MulMod64(acc,sqr,M);
		sqr=MWC_MulMod64(sqr,sqr,M);
		e=e>>1;
	}
	return acc;
}

uint2 MWC_SeedImpl_Mod64(ulong A, ulong M, uint vecSize, uint vecOffset, ulong streamBase, ulong streamGap)
{
	// This is an arbitrary constant for starting LCG jumping from. I didn't
	// want to start from 1, as then you end up with the two or three first values
	// being a bit poor in ones - once you've decided that, one constant is as
	// good as any another. There is no deep mathematical reason for it, I just
	// generated a random number.
	enum{ MWC_BASEID = 4077358422479273989UL };
	
	ulong dist=streamBase + (get_global_id(0)*vecSize+vecOffset)*streamGap;
	ulong m=MWC_PowMod64(A, dist, M);
	
	ulong x=MWC_MulMod64(MWC_BASEID, m, M);
	return (uint2)((uint)(x/A), (uint)(x%A));
}

void MWC64X_SeedStreams(mwc64x_state_t *s, ulong baseOffset, ulong perStreamOffset)
{
	uint2 tmp=MWC_SeedImpl_Mod64(MWC64X_A, MWC64X_M, 1, 0, baseOffset, perStreamOffset);
	s->x=tmp.x;
	s->c=tmp.y;
}



int randi(t_random *random)
{
    // uint seed = random->global_id.x * random->global_id.y + random->host_random_number + random->iteration;
    // uint t = seed ^ (seed << 11);  
    // uint result = random->global_id.y ^ (random->global_id.y >> 19) ^ (t ^ (t >> 8));
    // random->iteration++;
    // return (random->array[result%random->size]);
    return (1);
}

float randf(t_random *random)
{
    // uint seed = random->global_id.x * random->global_id.y + random->host_random_number + random->iteration;
    // uint t = seed ^ (seed << 11);  
    // uint result = random->global_id.y ^ (random->global_id.y >> 19) ^ (t ^ (t >> 8));
    // random->iteration++;
    // return ((float)(random->array[result%random->size]%9999999999)/9999999999);

    return ((float)MWC64X_NextUint(&random->state)/2147483648.0f/2.0f);
}

float3 rand_point_on_hemisphere(float r1, float r2)
{
    // float3 result;
    // float theta;
    // float phi;
    
    // theta = randf(random)*2.0f*M_PI_F;
    // phi = randf(random)*(M_PI_F/2.0f);
    // // phi = randf(random)*(M_PI_F);

    // result = (float3){
    //     sin(theta) * cos(phi),
    //     cos(theta),
    //     sin(theta) * sin(phi)
    // };
    // printf("%d\n", result.z < 0.0f);
    // return (result);
    float sinTheta = sqrt(1 - r1 * r1); 
    float phi = 2 * M_PI_F * r2; 
    float x = sinTheta * cos(phi); 
    float z = sinTheta * sin(phi);
    // printf("Vector((%f, %f, %f))\n", x, r1, z);
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