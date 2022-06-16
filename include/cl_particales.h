#ifndef CL_PARTICALES_H
#define CL_PARTICALES_H

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/opencl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CL_COUNT_PARTICLES 1000000

class CLEngine
{
private:
    glm::vec4 *vec3;
public:
    CLEngine();

    void InitParticles(cl_GLuint vbo);

    glm::vec4 GetVec3Array(int i){return vec3[i];};
    void GetMatPosicion(int i, glm::mat4 &model);
};


#endif