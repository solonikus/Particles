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
		// std::cout << "Device: " << device.getInfo<CL_DEVICE_EXTENSIONS>() << std::endl;
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
	// mem_buf = cl::Buffer(context, CL_MEM_READ_WRITE, CL_COUNT_PARTICLES * sizeof(cl_float3), NULL, &ret);
	cl::Program prog;
	cl::Kernel kern;
	std::ifstream sourceFile("E:\\CodeBase\\School21\\Particles\\shaders\\partical.cl");
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, {sourceCode.c_str(), sourceCode.length() + 1});
	cl::Program program = cl::Program(context, source);
	program.build(devices);
	cl::Kernel k_init(program, "build_cube");
	k_moving_wa = cl::Kernel(program, "move_without_atractor");
	k_init.setArg(0, mem_buf);
	k_moving_wa.setArg(0, mem_buf);
	queue.enqueueNDRangeKernel(k_init, cl::NullRange, cl::NDRange(CL_COUNT_PARTICLES), cl::NDRange(128));
	queue.finish();

	// vec3 = new vec3_print[CL_COUNT_PARTICLES];
	// queue.enqueueReadBuffer(mem_buf, CL_TRUE, 0, CL_COUNT_PARTICLES * sizeof(cl_float3), vec3);
	// for (int i = 0; i < 10; i++)
	// {
	// 	std::cout << "# "<< i << " x: " << vec3[i].x << " z: " << vec3[i].z << "\n";
	// }
	// int i;
	// i = 0;
}

void CLEngine::Rotate(double time)
{
	k_moving_wa.setArg(0, mem_buf);
	k_moving_wa.setArg(1, time);
	queue.enqueueNDRangeKernel(k_moving_wa, cl::NullRange, cl::NDRange(CL_COUNT_PARTICLES), cl::NDRange(128));
	queue.finish();

	// vec3 = new vec3_print[CL_COUNT_PARTICLES];
	// queue.enqueueReadBuffer(mem_buf, CL_TRUE, 0, CL_COUNT_PARTICLES * sizeof(cl_float3), vec3);
	// for (int i = 0; i < 10; i++)
	// {
	// 	std::cout << "# "<< i << " x: " << vec3[i].x << " z: " << vec3[i].z << "\n";
	// }
	// int i;
	// i = 0;
}