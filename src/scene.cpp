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

static void DrawSphere(int x, int y, std::vector<float> &vertices, std::vector<int> &indices, float size)
{
	for (int i = 0; i <= x; ++i)
	{
		float V   = i / (float) x;
		float phi = V * glm::pi <float> ();

		// Loop Through Slices
		for (int j = 0; j <= y; ++j){

			float U = j / (float) y;
			float theta = U * (glm::pi <float> () * 2.0f);

			// Calc The Vertex Positions
			float x = cosf(theta) * sinf(phi);
			float y = cosf(phi);
			float z = sinf(theta) * sinf(phi);

			// Push Back Vertex Data
			glm::vec3 point = glm::vec3(x, y, z) * size;
			vertices.push_back(point.x); vertices.push_back(point.y); vertices.push_back(point.z);
		}
	}
	for (int i = 0; i < y * x + y; ++i)
	{
		indices.push_back (i);
		indices.push_back (i + y + 1);
		indices.push_back (i + y);

		indices.push_back (i + y + 1);
		indices.push_back (i);
		indices.push_back (i + 1);
	}
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

void GLEngine::InputKeys(GLFWscrollfun scroll, GLFWmousebuttonfun mouse, GLFWcursorposfun pointer, GLFWkeyfun key)
{
	glfwSetScrollCallback(window, scroll);
	glfwSetMouseButtonCallback(window,
						[](GLFWwindow *window, int button, int action, int mode)
						{
							Scene *self = static_cast<Scene*>(glfwGetWindowUserPointer(window));
							self->MouseButtonCallback(window, button, action, mode);
						});
	glfwSetKeyCallback(window,
                        [](GLFWwindow* window, int key, int scancode, int action, int mode)
						{
							Scene *self = static_cast<Scene*>(glfwGetWindowUserPointer(window));
							self->BindKeys(key);
						});
}

/*---Scene---*/

Scene::Scene()
{
	glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &m_vao);
	glGenVertexArrays(1, &m_vao2);
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
	// DrawCube(m_array_vertex, m_array_indexes);
	// AddObject(e_Cube);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, CL_COUNT_PARTICLES * 6 * sizeof(float), NULL, GL_STREAM_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_array_indexes.size() * sizeof(int), m_array_indexes.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_particles.InitParticles(m_vbo);


	glBindVertexArray(m_vao2);
	glGenBuffers(1, &m_vbo_sphere);
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_sphere);
	std::vector<float> vertex;
	std::vector<int> index;
	DrawSphere(8, 8, vertex, index, 0.03f);
	size_indexes = index.size();
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), vertex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), index.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	settings.gravity_point.x = 0;
	settings.gravity_point.y = 0;
	settings.gravity_point.z = 0;
	m_camera.m_angles.x = 0;
	m_camera.m_angles.y = 0;
	// m_shaders.SetVectorIn(GetShaderID(), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f), "color");
	while (!glfwWindowShouldClose(window))
	{
		Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Scene::Draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// for (auto obj : m_objects)
		// {
		// 	m_shaders.SetMatrixIn(GetShaderID(), obj.GetObjMatrix(), "transform");
		// }

	m_particles.Main(glfwGetTime(), settings);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );
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
		// m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");
	m_shaders.SetVectorIn(GetShaderID(), glm::vec4(1.0f, 0.5f, 0.2f, 0.7f), "color");
	glDrawArrays(GL_POINTS, 0, CL_COUNT_PARTICLES);
	glBindVertexArray(m_vao2);
	m_shaders.SetVectorIn(GetShaderID(), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f), "color");
	glDrawElements(GL_TRIANGLES, size_indexes, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
	PrintFPS();

		// for (int i = 0; i < CL_COUNT_PARTICLES; i++)
		// { 
		// 	// glm::mat4 model = m_particles.GetMatPosicion(i);
		// 	// model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		// 	m_shaders.SetMatrixIn(GetShaderID(), m_particles.GetMatPosicion(i), "transform");
		// 	glDrawArrays(GL_POINTS, 0, 1);
		// }
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
		// glm::vec3 norm = glm::normalize(glm::vec3(camX, camY, camZ));
		// printf("x = %g, y = %g, z = %g\n", camX, camY, camZ);
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

void Scene::MouseButtonCallback(GLFWwindow *window, int button, int action, int mode)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
		glfwSetCursorPosCallback(window,
					[](GLFWwindow* window, double x, double y)
					{
						Scene *self = static_cast<Scene*>(glfwGetWindowUserPointer(window));
						self->MouseMove(window, x, y);
					});
	}
}

void Scene::MouseMove(GLFWwindow* window, double x, double y)
{
	float z;
	int rect[4];
	glGetIntegerv(GL_VIEWPORT, rect);
	y = rect[3] - y - 1;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	glm::vec4 view = {rect[0],rect[1],rect[2],rect[3]};
	// printf("x = %g, y = %g\n", x,y);
	// printf("x = %d, y = %d, z = %d, w = %d\n", rect[0],rect[1],rect[2],rect[3]);
	glm::vec3 out = glm::unProject(glm::vec3(x,y,z), glm::mat4(1.0f) * m_camera.GetLookMatrix(), m_camera.GetProjMatrix(), view);
	// Draw();
	// printf("x = %g, y = %g, z = %g\n", out.x,out.y,out.z);
}