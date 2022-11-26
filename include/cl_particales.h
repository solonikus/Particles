#ifndef CL_PARTICALES_H
#define CL_PARTICALES_H

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/opencl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CL_COUNT_MAIN_PARTICLES 1000000
#define CL_COUNT_ADD_PARTICLES 10000
#define CL_COUNT_PARTICLES (CL_COUNT_MAIN_PARTICLES + CL_COUNT_ADD_PARTICLES)

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
} vec_part;

struct scene_settings
{
	cl_float3 gravity_point;
};

class CLEngine
{
private:
    vec_part *vec3;
public:
    CLEngine();

    cl::CommandQueue queue;
    cl::Kernel k_moving_wa, k_build_sphere, k_calc_dist;
    cl::BufferGL mem_buf;

    void InitParticles(cl_GLuint vbo);
    void Main(double time, scene_settings settings, glm::vec3 center);
    void CreateSphere(glm::vec3 point);
    void CalcDist(glm::vec3 point);
};


#endif