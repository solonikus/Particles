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

void gravity(__global vec3 *pos, int i, float3 grav_point, double time);
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

__kernel void build_cube(__global vec3 *pos, int main_count, float3 center)
{
	int i = get_global_id(0);
	pos[i].x = rand(213 * i) + center.x;
	pos[i].y = rand(545 * i) + center.y;
	pos[i].z = rand(127 * i) + center.z;
	if (i < main_count)
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

__kernel void vectors_casatel(__global vec3 *pos, float3 center)
{
	int i = get_global_id(0);

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
	pos[i].s_x += new_x - pos[i].x;
	pos[i].s_z += new_z - pos[i].z;
}

__kernel void build_sphere(__global vec3 *pos, int main_count, float3 center)
{
	int i = get_global_id(0);
	if (i > main_count)
	{
		float z = linearRandom(-0.1f, 1.0f, 12);
		float angle = linearRandom(0.0f, 6.29f, 33);
		float radius = sqrt((0.1f - z) * z);

		pos[i].x = (radius * cos(angle)) + center.x;
		pos[i].y = (radius * sin(angle)) + center.y;
		pos[i].z = z + center.z;
		pos[i].t = 1.0f;
		pos[i].s_x = rand(234 * i) / 5000;
		pos[i].s_y = rand(525 * i) / 5000;
		pos[i].s_z = rand(132 * i) / 5000;
	}
	else
	{
		float minmax = 1.0f;
		float z = linearRandom(-1.0f, 1.0f, 12);
		float angle = linearRandom(0.0f, 6.29f, 33);
		float radius = sqrt((1.0f - z) * z);
		minmax *= 2;

		pos[i].x = (radius * cos(angle)) * minmax + center.x;
		pos[i].y = (radius * sin(angle)) * minmax + center.y;
		pos[i].z = z * minmax + center.z - (minmax / 2);
		pos[i].t = 11.0f;
		pos[i].s_x = rand(234 * i) / 5000;
		pos[i].s_y = rand(525 * i) / 5000;
		pos[i].s_z = rand(132 * i) / 5000;
	}
}

void gravity(__global vec3 *pos, int i, float3 grav_point, double time)
{
	double grav = 0.0005;
	float3 point;
	point.x = pos[i].x;
	point.y = pos[i].y;
	point.z = pos[i].z;
	float3 vec_dist = normalize(grav_point - point);
	float3 new_vec = vec_dist * (float)(grav);

	pos[i].s_x += new_vec.x * time;
	pos[i].s_y += new_vec.y * time;
	pos[i].s_z += new_vec.z * time;
}

__kernel void move_with_atractor(__global vec3 *pos, double time, float3 grav_point, int main_count, int add_count, float3 cursor)
{
	int i = get_global_id(0);

	if (i >= main_count)
		pos[i].t -= time;
	if (pos[i].t < 0)
	{
		pos[i].x = pos[i].y = pos[i].z = 0.0f;
		pos[i].t = 0.0f;
		return;
	}
	gravity(pos, i, grav_point, time);
	pos[i].d = sqrt((cursor.x - pos[i].x) * (cursor.x - pos[i].x) + (cursor.y - pos[i].y) * (cursor.y - pos[i].y) + (cursor.z - pos[i].z) * (cursor.z - pos[i].z));
	pos[i].x += pos[i].s_x;
	pos[i].y += pos[i].s_y;
	pos[i].z += pos[i].s_z;
}