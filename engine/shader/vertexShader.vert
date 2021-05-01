#version 330 core

layout (location = 0) in vec4 position; // the position variable has attribute position 0
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in float textureIndex;

out vec2  v_TextureCoordinate;
out float v_TextureIndex;

void main()
{
    gl_Position = position; // see how we directly give a vec3 to vec4's constructor
    v_TextureCoordinate = textureCoordinate;
    v_TextureIndex = textureIndex;
};
