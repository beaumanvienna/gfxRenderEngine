#version 330 core

layout (location = 0) in vec4 a_Position; // the position variable has attribute position 0
layout (location = 1) in vec2 a_TextureCoordinate;
layout (location = 2) in float a_TextureIndex;

out vec2  v_TextureCoordinate;
out float v_TextureIndex;

void main()
{
    gl_Position = a_Position; // see how we directly give a vec3 to vec4's constructor
    v_TextureCoordinate = a_TextureCoordinate;
    v_TextureIndex = a_TextureIndex;
};
