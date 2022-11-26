#version 330 core
layout (location = 0) in vec4 aPos;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 project;

uniform vec4 color;
uniform vec4 dist;
out vec4 ourColor;

void main()
{
   gl_Position = project * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
   vec4 col = color;
   float dist = sqrt((color.x - aPos.x) * (color.x - aPos.x) + (color.y - aPos.y) * (color.y - aPos.y) + (color.z - aPos.z) * (color.z - aPos.z)) / 3;
   float red = sqrt(aPos.x * aPos.x + aPos.y * aPos.y + aPos.z * aPos.z);
   vec4 col1;
   col1.x = 0.5 / dist;
   col1.y = 0.7 / dist;
   col1.z = dist;
   col1.w = 1.0;
   if (aPos.w < 0.1)
      col.w = 0.0f;
   if (color.x == 10.0f)
      ourColor = color;
   else
      ourColor = col1;
};