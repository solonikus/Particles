#include "scene.h"
#include "cl_particales.h"

void triangle(float a, float b)
{
	// z : x
	// float a = aa; float b = bb;
	float a1 = 0.2;
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
	std::cout << "a: "<< a << " b: " << b << " new_z: " << new_z << " new_x: " << new_x <<"\n";
}

int main()
{
	// float a,b;
	// a = b = 0.1;
	// triangle(a, b);
	// triangle(a * -1, b);
	// triangle(-1 * a, -1 * b);
	// triangle(a, -1 * b);
	// std::cout << "\n";
	// a = b = 0.5;
	// triangle(a, b);
	// triangle(a * -1, b);
	// triangle(-1 * a, -1 * b);
	// triangle(a, -1 * b);
	// return 1;


	try
	{
		Scene main_scene;
		main_scene.InitScene();
		// CLEngine cl_particles;
		main_scene.Loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
