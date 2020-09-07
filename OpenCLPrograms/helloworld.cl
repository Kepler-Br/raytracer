__kernel void main_kernel(__global int *array_to_change)
{
    int global_id = get_global_id(0);
    array_to_change[global_id] = 10;
}