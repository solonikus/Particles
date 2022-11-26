typedef struct 
{
	float x;
	float y;
	float z;
	float t;
	float d;
	float s_x;
	float s_y;
	float s_z;
} vec3;

void gravity(__global vec3 *pos, int i, float3 grav_point);
void new_vectors_casatel(__global vec3 *pos, int i);

float rand(int x)
{
	x = (x << 13) ^ x;
	return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

uint	random_number_generator(uint2 randoms)
{
	uint i = get_global_id(0);
	uint seed = (randoms.x + i * 168468498) ^ (randoms.y << 8);
	seed = (seed << 9) * seed;
	uint t = seed ^ (seed << 11);
	t = (t >> 5) ^ (t * 9135723);
	uint result = randoms.y ^ (randoms.y >> 19) ^ (t ^ (t >> 8));
	result = (result << 7) * 415645;

	return (result);
}

__kernel void build_cube(__global vec3 *pos, int main_count)
{
	int i = get_global_id(0);
	pos[i].x = rand(213 * i);
	pos[i].y = rand(545 * i);
	pos[i].z = rand(127 * i);
	if (i <= main_count)
		pos[i].t = 11.0f;
	else
		pos[i].t = 0.0f;
	pos[i].s_x = rand(234 * i) / 5000;
	pos[i].s_y = rand(525 * i) / 5000;
	pos[i].s_z = rand(132 * i) / 5000;
}

float		linearRandom(float min, float max, uint2 randoms)
{
	uint nb = random_number_generator(randoms);
	float ran = (nb / 4294967295.0f);

	return (ran * (max - min) + min);
}

__kernel void build_sphere(__global vec3 *pos, int main_count, float3 center)
{
	int i = get_global_id(0);
	float z = linearRandom(-0.1f, 1.0f, 12);
	float angle = linearRandom(0.0f, 6.29f, 33);
	float radius = sqrt((0.1f - z) * z);

	pos[i].x = (radius * cos(angle)) + center.x;
	pos[i].y = (radius * sin(angle)) + center.y;
	pos[i].z = z + center.z;
	pos[i].t = 100;
	pos[i].s_x = 0.0f;
	pos[i].s_y = 0.0f;
	pos[i].s_z = 0.0f;
}

// Треугольник. Прямоугольный. АВС. В - прямой угол.
// Вычисление координаты 3-й вершины С <xc_, yc_> по координатам вершин А <xa_, ya_>, В <xb_, yb_>
// и длине катета BC <bc_>
__kernel void move_without_atractor1(__global vec3 *pos, double time)
{
	int i = get_global_id(0);

	float a1 = 0.000001; //ускорение(касательная)
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
	float a1 = 0.0001; //ускорение(касательная)
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

void moving_center(__global vec3 *pos, int i, float3 center, bool move)
{
	if (move)
	{
		pos[i].x += center.x;
		pos[i].z += center.z;
		pos[i].y += center.y;
	}
	else
	{
		pos[i].x -= center.x;
		pos[i].z -= center.z;
		pos[i].y -= center.y;
	}
}

void gravity(__global vec3 *pos, int i, float3 grav_point)
{
	float a1 = 0.0000001;
	float3 point;
	point.x = pos[i].x;
	point.y = pos[i].y;
	point.z = pos[i].z;
	pos[i].d = fabs(distance(point, grav_point));
	float3 vec_dist = normalize(grav_point - point);
	float3 new_vec = vec_dist * a1;

	pos[i].s_x += new_vec.x;
	pos[i].s_y += new_vec.y;
	pos[i].s_z += new_vec.z;
}

__kernel void move_with_atractor(__global vec3 *pos, double time, float3 grav_point, int main_count, int add_count)
{
	int i = get_global_id(0);

	if (i >= main_count)
		pos[i].t -= 0.1;
	if (pos[i].t < 0)
	{
		pos[i].x = pos[i].y = pos[i].z = 0.0f;
		pos[i].t = 0.0f;
		return;
	}
	gravity(pos, i, grav_point);
	pos[i].x += pos[i].s_x;
	pos[i].y += pos[i].s_y;
	pos[i].z += pos[i].s_z;
	float3 point;
	point.x = pos[i].x;
	point.y = pos[i].y;
	point.z = pos[i].z;
}