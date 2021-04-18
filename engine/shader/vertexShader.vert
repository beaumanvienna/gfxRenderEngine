#version 330 core
layout (location = 0) in vec4 position; // the position variable has attribute position 0

void main()
{
    gl_Position = position; // see how we directly give a vec3 to vec4's constructor
}
