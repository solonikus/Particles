#include "scene.h"
#include "cl_particales.h"

int main()
{
	try
	{
		Scene main_scene;
		main_scene.InitScene();
		main_scene.Loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
