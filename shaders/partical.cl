
float rand(int x)
{
    x = (x << 13) ^ x;
    return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

__kernel void build_cube(__global float3 *pos)
{
    int i = get_global_id(0);
    pos[i].x = rand(213123 * i);
    pos[i].y = rand(545668 * i);
    pos[i].z = rand(127395 * i);
}