#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
       gl_Position = vec4(aPos.x/2, aPos.y/3, aPos.z/2, 1.0);
}