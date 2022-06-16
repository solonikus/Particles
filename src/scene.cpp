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
    window = glfwCreateWindow(g_width, g_height, "Particle System", nullptr, nullptr);
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

void GLEngine::InputKeys(GLFWscrollfun scroll, GLFWmousebuttonfun mouse, GLFWcursorposfun pointer, GLFWkeyfun key)
{
	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, pointer);
	glfwSetKeyCallback(window,
                        [](GLFWwindow* window, int key, int scancode, int action, int mode)
						{
							GLEngine *self = static_cast<GLEngine*>(glfwGetWindowUserPointer(window));
							self->BindKeys(key);
						});
}

void GLEngine::BindKeys(int key)
{
    return;
}

/*---Scene---*/

Scene::Scene()
{
	glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &m_vao);
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
	glBufferData(GL_ARRAY_BUFFER, CL_COUNT_PARTICLES * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_array_indexes.size() * sizeof(int), m_array_indexes.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	m_particles.InitParticles(m_vbo);
}

void Scene::AddObject(Objects obj)
{
	switch (obj)
	{
	case e_Ball:
		m_objects.emplace_back(e_Ball);
		break;
	case e_Cube:
		m_objects.emplace_back(e_Cube);
		break;
	case e_Plane:
		m_objects.emplace_back(e_Plane);
		break;	
	default:
		break;
	}
}

void Scene::Loop()
{
	// m_objects[0].Rotate(glm::vec3(1.0f), 5.0f);
	// m_objects[0].Move(glm::vec3(1.0f));
	// m_objects[0].Scale(glm::vec3(1.0f));
	
	glUseProgram(GetShaderID());
	m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetProjMatrix(), "project");
	glm::mat4 model;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// for (auto obj : m_objects)
		// {
		// 	m_shaders.SetMatrixIn(GetShaderID(), obj.GetObjMatrix(), "transform");
		// }
		glPointSize(1);
 		glBindVertexArray(m_vao);
		m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");
		m_shaders.SetMatrixIn(GetShaderID(), glm::mat4(1.0f), "transform");
		
		const float radius = 5.0f;
		const float speed = 0.5f;
		float camX = sin(glfwGetTime() * speed) * radius;
		float camZ = cos(glfwGetTime() * speed) * radius;
		m_camera.SetLookMatrix(glm::vec3(camX, 0.0f,  camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		int i = 0;
		glDrawArrays(GL_POINTS, 0, CL_COUNT_PARTICLES);

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