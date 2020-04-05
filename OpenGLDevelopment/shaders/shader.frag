#version 330 core

in vec3 positionColor;
out vec4 FragColor;

void main()
{
   FragColor = vec4(positionColor, 1.0f);
}