#ifndef SHADER_H
#define SHADER_H

// #include "scene.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "glm/mat4x4.hpp"

class ShaderManage
{
public:
	ShaderManage();
	~ShaderManage(){};

    int GetShaderId(){return m_shader_program;};
    void SetMatrixIn(int id, glm::mat4 matrix, std::string name);
    void SetVectorIn(int id, glm::vec4 matrix, std::string name);

private:
    int m_shader_program;
};




#endif