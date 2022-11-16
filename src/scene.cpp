#include "scene.h"

/*---Globar Vars---*/

int g_width = 1280;
int g_height = 720;

/*---Static Functions---*/

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void DrawCube(std::vector<float> &array_vertex, std::vector<int> &array_indexes)
{
    // std::vector<float> vertices {
    //     0.5f,  0.5f, -0.5f,
    //     0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f,  0.5f, -0.5f,
    //     -0.5f, -0.5f, 0.5f,
    //     -0.5f, 0.5f, 0.5f,
    //     0.5f, 0.5f, 0.5f,
    //     0.5f, -0.5f, 0.5f,

    // };
	    std::vector<float> vertices {
        0.0f,  0.0f, 0.0f
    };
    std::vector<int> indexes {
        0, 1, 3,  // первый треугольник
        1, 2, 3,  // второй треугольник
        3, 2, 4,
        4, 3, 5,
        4, 5, 7,
        5, 7, 6,
        6, 7, 0,
        7, 1, 0,
        1, 2, 7,
        2, 7, 4,
        0, 3, 6,
        3, 6, 5
    };
    array_vertex.insert(array_vertex.end(), vertices.begin(), vertices.end());
    array_indexes.insert(array_indexes.end(), indexes.begin(), indexes.end());
}

static float GetFPS()
{
	static double prev_time;
	double shot_time;
	float count_fps;

	shot_time = glfwGetTime() - prev_time;
	count_fps = 1.f / shot_time;	
	prev_time = glfwGetTime();
	return count_fps;
}

/*---Engine---*/

GLEngine::GLEngine()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(g_width, g_height, WINDOW_NAME, nullptr, nullptr);
    if (window == nullptr)
	{
		glfwTerminate();
		throw std::runtime_error("Can't create window\n");
	}
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowUserPointer(window, this);
    InputKeys(nullptr, nullptr, nullptr, nullptr);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("GLAD ERROR: Can't load glad");
	std::cout << glGetString(GL_RENDERER) << std::endl;
}

static void MouseMoves(GLFWwindow* window, double xpos, double ypos)
{

}

void GLEngine::InputKeys(GLFWscrollfun scroll, GLFWmousebuttonfun mouse, GLFWcursorposfun pointer, GLFWkeyfun key)
{
	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window,
						[](GLFWwindow *window, int button, int action, int mode)
						{
							GLEngine *self = static_cast<GLEngine*>(glfwGetWindowUserPointer(window));
							self->MouseButtonCallback(window, button, action, mode);
						});
	glfwSetKeyCallback(window,
                        [](GLFWwindow* window, int key, int scancode, int action, int mode)
						{
							Scene *self = static_cast<Scene*>(glfwGetWindowUserPointer(window));
							self->BindKeys(key);
						});
}

void GLEngine::MouseButtonCallback(GLFWwindow *window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
		glfwSetCursorPosCallback(window,
					[](GLFWwindow* window, double x, double y)
					{
						GLEngine *self = static_cast<GLEngine*>(glfwGetWindowUserPointer(window));
						self->MouseMove(window, x, y);
					});
	}
}

void GLEngine::MouseMove(GLFWwindow* window, double x, double y)
{
	int i;
	i = 0;
}

/*---Scene---*/

Scene::Scene()
{
	glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &m_vao);
	glfwSetWindowUserPointer(window, this);
	// settings.gravity_point.x = 0;
	// settings.gravity_point.y = 0;
	// settings.gravity_point.z = 0;
}

Scene::~Scene()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void Scene::InitScene()
{
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	// glGenBuffers(1, &m_ebo);
	// DrawCube(m_array_vertex, m_array_indexes);
	// AddObject(e_Cube);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, CL_COUNT_PARTICLES * 6 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_array_indexes.size() * sizeof(int), m_array_indexes.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	m_particles.InitParticles(m_vbo);
}

void Scene::AddObject(Objects obj)
{
	// switch (obj)
	// {
	// case e_Ball:
	// 	m_objects.emplace_back(e_Ball);
	// 	break;
	// case e_Cube:
	// 	m_objects.emplace_back(e_Cube);
	// 	break;
	// case e_Plane:
	// 	m_objects.emplace_back(e_Plane);
	// 	break;	
	// default:
	// 	break;
	// }
}

void Scene::Loop()
{
	// m_objects[0].Rotate(glm::vec3(1.0f), 5.0f);
	// m_objects[0].Move(glm::vec3(1.0f));
	// m_objects[0].Scale(glm::vec3(1.0f));
	
	glUseProgram(GetShaderID());
	// m_camera.SetLookMatrix(glm::vec3(0.0f, 0.0f,  0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetProjMatrix(), "project");
	m_shaders.SetMatrixIn(GetShaderID(), glm::mat4(1.0f), "transform");
	glPointSize(1);
	// m_camera.SetLookMatrix(glm::vec3(5.0, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// for (auto obj : m_objects)
		// {
		// 	m_shaders.SetMatrixIn(GetShaderID(), obj.GetObjMatrix(), "transform");
		// }
 		glBindVertexArray(m_vao);
		// glm::mat4 trans = glm::mat4(1.0f);
		// trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		// trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");
		// m_shaders.SetMatrixIn(GetShaderID(), trans, "transform");
		// const float radius = 5.0f;
		// const float speed = 0.5f;
		// float camX = sin(glfwGetTime() * speed) * radius;
		// float camZ = cos(glfwGetTime() * speed) * radius;	
		// m_camera.SetLookMatrix(glm::vec3(camX, camZ, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// m_camera.SetLookMatrix(m_camera.GetCameraView(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");

		glDrawArrays(GL_POINTS, 0, CL_COUNT_PARTICLES);
		PrintFPS();

		// for (int i = 0; i < CL_COUNT_PARTICLES; i++)
		// { 
		// 	// glm::mat4 model = m_particles.GetMatPosicion(i);
		// 	// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// 	m_shaders.SetMatrixIn(GetShaderID(), m_particles.GetMatPosicion(i), "transform");
		// 	glDrawArrays(GL_POINTS, 0, 1);
		// }
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Scene::PrintFPS()
{
	static double prev_time = 0;
	float fps = GetFPS();
	if ((glfwGetTime() - prev_time) < 1)
		return;
	prev_time = glfwGetTime();
	std::string title_name;
	title_name = std::string(WINDOW_NAME) + ("    ") + ("FPS: ") + std::to_string(fps);
	glfwSetWindowTitle(window, title_name.c_str());
}

void Scene::BindKeys(int key)
{
	auto update = [&](float angleX, float angleY)
	{
		const float radius = 5.0f;
		m_camera.m_angles.x += angleX;
		m_camera.m_angles.y += angleY;
		if (m_camera.m_angles.x > 89.0)
		{
			m_camera.m_angles.x = 89.0;
		}
		else if (m_camera.m_angles.x < -89.0)
		{
			m_camera.m_angles.x = -89.0;
		}
		float camX = cos(glm::radians(m_camera.m_angles.x)) * cos(glm::radians(m_camera.m_angles.y)) * radius;
		float camY = sin(glm::radians(m_camera.m_angles.x)) * radius;
		float camZ = cos(glm::radians(m_camera.m_angles.x)) * sin(glm::radians(m_camera.m_angles.y)) * radius;
		m_camera.SetLookMatrix({camX, camY, camZ}, {0.0f, 0.0f, 0.0f}, {0.0, 1.0, 0.0});
	};
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		update(1.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		update(-1.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		update(0.0, 1.0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		update(0.0, -1.0);
	}
	m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");
}