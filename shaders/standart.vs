#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in float dist;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

uniform vec4 color;
out vec4 ourColor;

void main()
{
   gl_Position = project * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
   vec4 col = color;
   col.x = dist / 2;
   col.y = 0.5;
   col.z = dist / 0.5;
   col.w = 0.5;
   if (color.x > 10.0f)
      col.x = 0.7; col.y = 0.5; col.z = 0.9; col.w = 1.0;
   if (color.x < 10.0f && aPos.w < 0.1)
      col.w = 0.0f;
   ourColor = col;
};