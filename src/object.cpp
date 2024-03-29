
#include <stdio.h>
#include "object.h"

extern int g_width;
extern int g_height;

/*---Static Functions---*/


/*---Object---*/
Object::Object() :  m_position(1.0f)
{
}

Object::Object(Objects obj) :  m_position(1.0f)
{
}

Object::~Object()
{
}

void Object::Move(glm::vec3 vec_move)
{
	m_position = glm::translate(m_position, vec_move);
}

void Object::Scale(glm::vec3 scale_vec)
{
	m_position = glm::scale(m_position, scale_vec);
}

void Object::Rotate(glm::vec3 rot_vec, float angle)
{
    m_position = glm::rotate(m_position, angle, rot_vec);
}

glm::mat4 Object::GetObjMatrix()
{
	return m_position;
}


/*---Camera---*/


Camera::Camera()
{
	m_proj_matrix = glm::perspective(glm::radians(45.0f), (float)g_width/(float)g_height, 0.1f, 100.0f);
	SetLookMatrix(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetProjMatrix()
{
	return m_proj_matrix;
}

glm::mat4 Camera::GetLookMatrix()
{
	return m_look_at;
}

glm::mat4 Camera::SetLookMatrix(glm::vec3 rotate, glm::vec3 up, glm::vec3 eye)
{
	m_look_at = glm::lookAt(rotate, up, eye); //1 - rotate, 2 vverx, 3 napravlenie
	return glm::mat4(1.f);
}