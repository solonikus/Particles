#ifndef CL_PARTICALES_H
#define CL_PARTICALES_H

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/opencl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CL_COUNT_PARTICLES 100000

typedef struct 
{
    float x;
    float y;
    float z;
    float s_x;
    float s_y;
    float s_z;
} vec3_print;

struct scene_settings
{
	cl_float3 gravity_point;
};

class CLEngine
{
private:
    vec3_print *vec3;
public:
    CLEngine();

    cl::CommandQueue queue;
    cl::Kernel k_moving_wa, k_move_x, k_move_y;
    cl::BufferGL mem_buf;

    void InitParticles(cl_GLuint vbo);
    void Main(double time, scene_settings settings);
    void Rotate(bool x, float angle);

    // glm::vec4 GetVec3Array(int i){return vec3[i];};
    void GetMatPosicion(int i, glm::mat4 &model);
};


#endif