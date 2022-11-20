#version 330 core
in  vec4 ourColor;
out vec4 FragColor;
void main()
{
    FragColor = ourColor;
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 0.0f);
};