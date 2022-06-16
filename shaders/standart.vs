#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;
void main()
{
   //gl_Position = project * view * transform * vec4(aPos, 1.0f);
   gl_Position = transform * vec4(aPos, 1.0f);
};