#include "shader.h"

#include "glm/gtc/type_ptr.hpp"
/*---Static Functions---*/

std::string readFile(const std::string& fileName) 
{
    std::ifstream f(fileName);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

/*---ShaderManage---*/

ShaderManage::ShaderManage()
{
    // Компилирование нашей шейдерной программы

    // Вершинный шейдер
    std::string shader_source(readFile("E:\\CodeBase\\School21\\Particles\\shaders\\standart.vs"));
    const char* const_shader_source = shader_source.c_str();
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &const_shader_source, NULL);
    glCompileShader(vertexShader);
 
    // Проверка на наличие ошибок компилирования вершинного шейдера
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
 
    // Фрагментный шейдер
    shader_source = readFile("E:\\CodeBase\\School21\\Particles\\shaders\\standart.fs");
    const_shader_source = shader_source.c_str();
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &const_shader_source, NULL);
    glCompileShader(fragmentShader);
 
    // Проверка на наличие ошибок компилирования фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
 
    // Связывание шейдеров
    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, vertexShader);
    glAttachShader(m_shader_program, fragmentShader);
    glLinkProgram(m_shader_program);
 
    // Проверка на наличие ошибок компилирования связывания шейдеров
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(m_shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderManage::SetMatrixIn(int id, glm::mat4 matrix, std::string name)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderManage::SetVectorIn(int id, glm::vec4 vec, std::string name)
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}