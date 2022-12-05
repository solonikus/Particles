#include "scene.h"

/*---Globar Vars---*/

int g_width = 1280;
int g_height = 720;

/*---Static Functions---*/

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	glfwSetCursorPosCallback(window,
						[](GLFWwindow* window, double x, double y)
						{
							Scene *self = static_cast<Scene*>(glfwGetWindowUserPointer(window));
							self->MouseMove(window, x, y);
						});
}

/*---Scene---*/

Scene::Scene()
{
	glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &m_vao);
	glGenVertexArrays(1, &m_vao2);
	glfwSetWindowUserPointer(window, this);
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

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, CL_COUNT_PARTICLES * sizeof(vec_part), NULL, GL_STREAM_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec_part), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(vec_part), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Scene::Loop()
{	
	glUseProgram(GetShaderID());
	m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetProjMatrix(), "project");
	m_shaders.SetMatrixIn(GetShaderID(), glm::mat4(1.0f), "transform");
	glPointSize(1);
	m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");
	settings.gravity_point.x = 0;
	settings.gravity_point.y = 0;
	settings.gravity_point.z = 0;
	m_camera.m_angles.x = 0;
	m_camera.m_angles.y = 0;
	m_camera.m_angles.z = 5.0f;
	double lasttime = 0.0;
	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();
		double delta = time - lasttime;
		Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Scene::Draw(char mode)
{	
	static double time;
	static double curtime;
	glm::vec4 col(m_color, 1.0f);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (mode == 0)
	{
		time = fabs(time - glfwGetTime());
		m_particles.Main(time, settings, m_cursor);
		time = glfwGetTime();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );
		glBindVertexArray(m_vao);
		m_shaders.SetMatrixIn(GetShaderID(), glm::mat4(1.0f), "transform");
		m_shaders.SetVectorIn(GetShaderID(), col, "color");
			glDrawArrays(GL_POINTS, 0, CL_COUNT_PARTICLES);
	}

	glBindVertexArray(m_vao2);
	col.x = 11.0f;
	if (!m_is_grav_center_vis && !m_is_grav_center_vis_true)
	{
		col.w = 0.0;
		col.x = 1.0f;
	}
	m_shaders.SetMatrixIn(GetShaderID(), m_object.GetObjMatrix(), "transform");
	m_shaders.SetVectorIn(GetShaderID(), col, "color");
	glDrawElements(GL_TRIANGLES, size_indexes, GL_UNSIGNED_INT, 0);
	if (mode == 0)
	{
		glDisable(GL_BLEND);

		PrintFPS();
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
	auto update = [&](float angleX, float angleY, float rad)
	{
		m_camera.m_angles.x += angleX;
		m_camera.m_angles.y += angleY;
		m_camera.m_angles.z += rad;
		float radius = m_camera.m_angles.z;
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
		m_shaders.SetMatrixIn(GetShaderID(), m_camera.GetLookMatrix(), "view");
	};
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		update(1.0, 0.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		update(-1.0, 0.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		update(0.0, 1.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		update(0.0, -1.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		update(0.0, 0.0, 0.1);
		m_dist += 0.0002;
	}
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		update(0.0, 0.0, -0.1);
		m_dist -= 0.0002;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		m_particles.Build(true, settings.gravity_point);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_particles.Build(false, settings.gravity_point);
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		m_is_grav_center_vis_true = m_is_grav_center_vis_true ? false : true;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_particles.VecQuas(settings.gravity_point);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
			m_object.m_position = glm::mat4(1.0f);
			settings.gravity_point.x = 0.0f;
			settings.gravity_point.y = 0.0f;
			settings.gravity_point.z = 0.0f;
	}
}

void Scene::MouseButtonCallback(GLFWwindow *window, int button, int action, int mode)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		double x, y;
	   	glfwGetCursorPos(window, &x, &y);
		float z;
		int rect[4];
		glGetIntegerv(GL_VIEWPORT, rect);
		y = rect[3] - y - 1;
		glm::vec4 view = {rect[0],rect[1],rect[2],rect[3]};
		glm::vec3 camera{m_camera.GetLookMatrix()[3][0], m_camera.GetLookMatrix()[3][1], m_camera.GetLookMatrix()[3][2]};
		z = glm::distance(camera, glm::vec3(0.0f));
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
		// printf("x = %g, y = %g, z = %g\n", x, y, z);
		glm::vec3 out = glm::unProject(glm::vec3(x,y,0.977), glm::mat4(1.0f) * m_camera.GetLookMatrix(), m_camera.GetProjMatrix(), view);
		m_particles.CreateSphere(out);
		// printf("nx = %g, ny = %g, nz = %g\n", out.x,out.y,out.z);
	}
}

void Scene::MouseMove(GLFWwindow* window, double x, double y)
{
	float z;
	static float tmp;
	int rect[4];
	glGetIntegerv(GL_VIEWPORT, rect);
	y = rect[3] - y - 1;
	Draw(1);
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	glm::vec4 view = {rect[0],rect[1],rect[2],rect[3]};
	glm::vec3 out = glm::unProject(glm::vec3(x,y,z), glm::mat4(1.0f) * m_camera.GetLookMatrix(), m_camera.GetProjMatrix(), view);
	if(!(fabs(out.x) < 6.0f && fabs(out.y) < 6.0f && fabs(out.z) < 6.0f))
		m_is_grav_center_vis = false;
	else
		m_is_grav_center_vis = true;
	auto newcoord = [&]()
	{
		m_is_grav_center_vis = true;
		glm::vec3 out = glm::unProject(glm::vec3(x,y,tmp), glm::mat4(1.0f) * m_camera.GetLookMatrix(), m_camera.GetProjMatrix(), view);
		if(fabs(out.x) < 6.0f && fabs(out.y) < 6.0f && fabs(out.z) < 6.0f)
		{
			m_object.m_position = glm::mat4(1.0f);
			m_object.Move({out.x, out.y, out.z});
			settings.gravity_point.x = m_object.m_position[3][0];
			settings.gravity_point.y = m_object.m_position[3][1];
			settings.gravity_point.z = m_object.m_position[3][2];
		}
	};
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		newcoord();
	}
	else
	{
		tmp = z;
	}
	m_cursor = glm::unProject(glm::vec3(x,y,m_dist), glm::mat4(1.0f) * m_camera.GetLookMatrix(), m_camera.GetProjMatrix(), view);
}