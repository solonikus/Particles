
#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "emuns.h"
#include "object.h"
#include "shader.h"
#include "cl_particales.h"

#define WINDOW_NAME "Particles System"


class GLEngine
{
public:
	GLEngine();
	~GLEngine(){};

	GLuint m_vao, m_vao2,  m_vbo, m_ebo, m_vbo_sphere;
	int size_indexes;
	GLFWwindow   *window;

private:

	void InputKeys(GLFWscrollfun, GLFWmousebuttonfun, GLFWcursorposfun, GLFWkeyfun);
};

class Scene : public GLEngine
{
public:
	Scene();
	~Scene();

	Object				m_object;
	ShaderManage		m_shaders;
	Camera				m_camera;
	CLEngine			m_particles;

	std::vector<float>	m_array_vertex;
	std::vector<int>	m_array_indexes;
	glm::vec3 			m_color;
	glm::vec3 			m_cursor;

	scene_settings		settings;
	bool				m_is_fps_enable = 1, m_is_grav_center_vis = 1;

	void InitScene();
	void AddObject(Objects obj);
	void Loop();
	void Draw(char mode = 0);

	void PrintFPS();

	int GetShaderID(){return m_shaders.GetShaderId();};

	void BindKeys(int);
	void MouseMove(GLFWwindow* window, double x, double y);
	void MouseButtonCallback(GLFWwindow *window, int button, int action, int mode);
};

#endif //SCENE_H