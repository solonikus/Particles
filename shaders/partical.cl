typedef struct 
{
    float x;
    float y;
    float z;
    float s_x;
    float s_y;
    float s_z;
} vec3;

float rand(int x)
{
    x = (x << 13) ^ x;
    return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

__kernel void build_cube(__global vec3 *pos)
{
    int i = get_global_id(0);
    pos[i].x = rand(213123 * i);
    pos[i].y = rand(545668 * i);
    pos[i].z = rand(127395 * i);
    // pos[i].s_x = atan(pos[i].x / pos[i].z);
    // pos[i].s_y = atan(pos[i].x / pos[i].z);
    // pos[i].s_z = atan(pos[i].z / pos[i].x);
    pos[i].s_x = 0.001;
    pos[i].s_y = 0;
    pos[i].s_z = 0.001;
}

// __kernel void move_without_atractor(__global vec3 *pos, double time)
// {
//     int i = get_global_id(0);
//     float radius = sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z));
//     float angle_x = acos(pos[i].x / radius);
//     float angle_z = asin(pos[i].z / radius);
//     float gradus = (M_PI * time) / 180;
//     pos[i].x = sin(time + pos[i].s_x) * radius;

//     pos[i].z = cos(time + pos[i].s_x) * radius;
// }

// __kernel void move_without_atractor(__global vec3 *pos, double time)
// {
//     int i = get_global_id(0);

//     float radius = sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z));
//     float procent = (0.001 / radius * 100) / 100;
//     float3 grav, velo;
//     grav.x = (fabs(pos[i].x) - fabs(pos[i].x * procent)) * (pos[i].x > 0 ? 1 : -1);
//     // grav.y = pos[i].y - pos[i].y * procent;
//     grav.z = (fabs(pos[i].z) - fabs(pos[i].z * procent)) * (pos[i].z > 0 ? 1 : -1);


//     // float xa = pos[i].x;
//     // float ya = pos[i].z;
//     // float xb = grav.x;
//     // float yb = grav.z;
//     // float x2x1 = xa - xb;
//     // float y2y1 = ya - yb;
//     // // float x2x1 = pos[i].x - grav.x;
//     // // float y2y1 = pos[i].z - grav.z;
//     // // float ab   = sqrt(x2x1*x2x1 + y2y1*y2y1);
//     // float ab   = 0.001;
//     // float v1x  = (xa - xb) / ab;
//     // float v1y  = (ya - yb) / ab;
//     // float v3x  = v1y  * 0.001;
//     // float v3y  = v1x  * 0.001;
//     // float xc = xb + v3x;
//     // float yc = yb + v3y;
//     // velo.x = xc;
//     // velo.y = 0;
//     // velo.z = yc;

//     pos[i].x = grav.x + pos[i].s_x;
//     // pos[i].y = grav.y;
//     pos[i].z = grav.z + pos[i].s_z;
//     // pos[i].x = grav.x;
//     // // pos[i].y = grav.y;
//     // pos[i].z = grav.z;
// }

// Треугольник. Прямоугольный. АВС. В - прямой угол.
// Вычисление координаты 3-й вершины С <xc_, yc_> по координатам вершин А <xa_, ya_>, В <xb_, yb_>
// и длине катета BC <bc_>
__kernel void move_without_atractor(__global vec3 *pos, double time)
{
    int i = get_global_id(0);
    float a1 = 0.05;

    // float radius = sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z));
    // float procent = (0.001 / radius * 100) / 100;
    // float3 grav, velo;
    // grav.x = (fabs(pos[i].x) - fabs(pos[i].x * procent)) * (pos[i].x > 0 ? 1 : -1);
    // // grav.y = pos[i].y - pos[i].y * procent;
    // grav.z = (fabs(pos[i].z) - fabs(pos[i].z * procent)) * (pos[i].z > 0 ? 1 : -1);

    float a = pos[i].z; float b = pos[i].x;
	int sign_a = a >= 0 ? 1 : -1; int sign_b = b >= 0 ? 1 : -1;
	float radius = sqrt((a * a) + (b * b));
	float c = radius;
    float angle_a = atan(a/b) * 180 / M_PI;
    float angle_a1 = atan(a1/c) * 180 / M_PI;
    float c1 = sqrt((c * c) + (a1 * a1));
    float angle_a2 = angle_a - angle_a1;
    float angle_b2 = 90 - angle_a2;
    float new_z = c1 * sin((angle_a2) * M_PI / 180) * sign_b;
    float new_x = c1 * sin((angle_b2) * M_PI / 180) * sign_b;
    pos[i].x = new_x;
    pos[i].z = new_z;
    
    radius = sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z));
    float procent = a1 / radius / 100;
    float3 grav;
    grav.x = (fabs(pos[i].x) - fabs(pos[i].x * procent)) * (pos[i].x > 0 ? 1 : -1);
    grav.z = (fabs(pos[i].z) - fabs(pos[i].z * procent)) * (pos[i].z > 0 ? 1 : -1);
    pos[i].x = grav.x;
    pos[i].z = grav.z;
}