typedef struct 
{
    float x;
    float y;
    float z;
    float s_x;
    float s_y;
    float s_z;
} vec3;

void gravity(__global vec3 *pos, int i);
void new_vectors_casatel(__global vec3 *pos, int i);

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
    pos[i].x = 1;
    pos[i].y = 1;
    pos[i].z = 1;
    // pos[i].s_x = atan(pos[i].x / pos[i].z);
    // pos[i].s_y = atan(pos[i].x / pos[i].z);
    // pos[i].s_z = atan(pos[i].z / pos[i].x);
    pos[i].s_x = 0;
    pos[i].s_y = 0;
    pos[i].s_z = 0;
    // new_vectors_casatel(pos, i);
    // gravity(pos, i);
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


// Треугольник. Прямоугольный. АВС. В - прямой угол.
// Вычисление координаты 3-й вершины С <xc_, yc_> по координатам вершин А <xa_, ya_>, В <xb_, yb_>
// и длине катета BC <bc_>
__kernel void move_without_atractor1(__global vec3 *pos, double time)
{
    int i = get_global_id(0);

    // float radius = sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z));
    // float procent = (0.001 / radius * 100) / 100;
    // float3 grav, velo;
    // grav.x = (fabs(pos[i].x) - fabs(pos[i].x * procent)) * (pos[i].x > 0 ? 1 : -1);
    // // grav.y = pos[i].y - pos[i].y * procent;
    // grav.z = (fabs(pos[i].z) - fabs(pos[i].z * procent)) * (pos[i].z > 0 ? 1 : -1);

    float a1 = 0.001; //ускорение(касательная)
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
    
    float new_radius = sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z));
    float a2 = radius - new_radius;
    float procent = a2 / new_radius;
    float3 grav;
    grav.x = (fabs(pos[i].x) - fabs(pos[i].x * procent)) * (pos[i].x > 0 ? 1 : -1);
    grav.z = (fabs(pos[i].z) - fabs(pos[i].z * procent)) * (pos[i].z > 0 ? 1 : -1);
    pos[i].x = grav.x;
    pos[i].z = grav.z;

    if (i == 10)
    {
        printf("x = %g , z = %g , len = %g, a1=%f", pos[i].x, pos[i].z, sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z)), a1);
    }
}

void new_vectors_casatel(__global vec3 *pos, int i)
{
    float a1 = 0.001; //ускорение(касательная)
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
    pos[i].s_x = new_x - pos[i].x;
    pos[i].s_z = new_z - pos[i].z;

    float new_radius = sqrt((new_x * new_x) + (new_z * new_z));
    float a2 = radius - new_radius;
    pos[i].s_y = a2;
}

void new_vectors(__global vec3 *pos, int i)
{

}


void gravity(__global vec3 *pos, int i)
{
    float a1 = 0.00001;
    // a1 = pos[i].s_y;
    int sign_x = pos[i].x >= 0 ? -1 : 1;
    int sign_z = pos[i].z >= 0 ? -1 : 1;
    float a = pos[i].x + pos[i].s_x, b = pos[i].z + pos[i].s_z;
    // float a = pos[i].x, b = pos[i].z;
    float angle_a = fabs(atan(a/b) * 180 / M_PI);
    float new_z = a1 * cos((angle_a) * M_PI / 180) * sign_z;
    float new_x = a1 * sin((angle_a) * M_PI / 180) * sign_x;
    pos[i].s_x += new_x;
    pos[i].s_z += new_z;
    // if (i == 10)
    // {
    //     printf("x = %g , z = %g , new_x = %g , new_z = %g", pos[i].x, pos[i].z, new_x , new_z);
    // }
}

__kernel void move_without_atractor(__global vec3 *pos, double time)
{
    int i = get_global_id(0);
    new_vectors(pos, i);
    new_vectors_casatel(pos, i);
    gravity(pos, i);
    if (i == 10)
    {
        printf("x = %g , z = %g , len = %g", pos[i].x, pos[i].z, sqrt((pos[i].x * pos[i].x) + (pos[i].z * pos[i].z)));
    }
    pos[i].x += pos[i].s_x;
    pos[i].z += pos[i].s_z;

    // if (pos[i].s_x == 0 && pos[i].s_y == 0)
    // {
        
    // }
}