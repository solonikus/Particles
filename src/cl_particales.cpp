#include "cl_particales.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <wingdi.h>

CLEngine::CLEngine()
{
}

void CLEngine::InitParticles(cl_GLuint vbo)
{
	cl_int ret;
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	std::vector<cl::Device> devices;
	for(auto &platform : platforms)
	{
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	}
	char str[128];
	size_t retSize;
	cl_int error;
	for (auto device : devices)
	{
		std::cout << "Device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
	}
	cl::Platform pltf = platforms[0];
	cl_context_properties props[] = {
		CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[0]()),
		CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentContext()),
		CL_WGL_HDC_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentDC()),
		0};
	cl::Context context(devices[0], props);
	queue = cl::CommandQueue(context, devices[0]);
	mem_buf = cl::BufferGL(context, CL_MEM_READ_WRITE, vbo, &ret);
	cl::Program prog;
	cl::Kernel kern;
	std::ifstream sourceFile("E:\\CodeBase\\School21\\Particles\\shaders\\partical.cl");
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, {sourceCode.c_str(), sourceCode.length() + 1});
	cl::Program program = cl::Program(context, source);
	program.build(devices);
	k_init = cl::Kernel(program, "build_cube");
	k_moving_wa = cl::Kernel(program, "move_with_atractor");
	k_build_sphere = cl::Kernel(program, "build_sphere");
	k_vector_kas = cl::Kernel(program, "vectors_casatel");
	k_init.setArg(0, mem_buf);
	k_init.setArg(1, CL_COUNT_MAIN_PARTICLES);
	cl_float3 cl_cent{0.0f, 0.0f, 0.0f};
	k_init.setArg(2, cl_cent);
	queue.enqueueNDRangeKernel(k_init, cl::NullRange, cl::NDRange(CL_COUNT_PARTICLES), cl::NDRange(128));
	queue.finish();
}

void CLEngine::Main(double time, scene_settings settings, glm::vec3 center)
{
	cl_float3 cl_cent;
	cl_cent.x = center.x;
	cl_cent.y = center.y;
	cl_cent.z = center.z;
	k_moving_wa.setArg(0, mem_buf);
	k_moving_wa.setArg(1, time);
	k_moving_wa.setArg(2, settings);
	k_moving_wa.setArg(3, CL_COUNT_MAIN_PARTICLES);
	k_moving_wa.setArg(4, CL_COUNT_ADD_PARTICLES);
	k_moving_wa.setArg(5, cl_cent);
	queue.enqueueNDRangeKernel(k_moving_wa, cl::NullRange, cl::NDRange(CL_COUNT_PARTICLES), cl::NDRange(128));
	queue.finish();
}

void CLEngine::CreateSphere(glm::vec3 center)
{
	static int number = 0;
	if (number == 100)
		number = 0;
	cl_float3 cl_cent;
	cl_cent.x = center.x;
	cl_cent.y = center.y;
	cl_cent.z = center.z;
	k_build_sphere.setArg(0, mem_buf);
	k_build_sphere.setArg(1, CL_COUNT_MAIN_PARTICLES);
	k_build_sphere.setArg(2, cl_cent);
	queue.enqueueNDRangeKernel(k_build_sphere, cl::NDRange(CL_COUNT_MAIN_PARTICLES + number * 1000), cl::NDRange(1000), cl::NDRange(128));
	queue.finish();
	number++;
}

void CLEngine::Build(bool is_cube, cl_float3 center)
{
	if (is_cube)
	{
		k_init.setArg(0, mem_buf);
		k_init.setArg(1, CL_COUNT_MAIN_PARTICLES);
		k_init.setArg(2, center);
		queue.enqueueNDRangeKernel(k_init, cl::NullRange, cl::NDRange(CL_COUNT_MAIN_PARTICLES), cl::NDRange(128));
		queue.finish();
	}
	else
	{
		k_build_sphere.setArg(0, mem_buf);
		k_build_sphere.setArg(1, CL_COUNT_MAIN_PARTICLES);
		k_build_sphere.setArg(2, center);
		queue.enqueueNDRangeKernel(k_build_sphere, cl::NullRange, cl::NDRange(CL_COUNT_MAIN_PARTICLES), cl::NDRange(128));
		queue.finish();
	}
}

void CLEngine::VecQuas(cl_float3 center)
{
	k_vector_kas.setArg(0, mem_buf);
	k_vector_kas.setArg(1, center);
	queue.enqueueNDRangeKernel(k_vector_kas, cl::NullRange, cl::NDRange(CL_COUNT_MAIN_PARTICLES), cl::NDRange(128));
	queue.finish();
}