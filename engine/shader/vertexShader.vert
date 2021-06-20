#version 330 core

layout (location = 0) in vec4 a_Position; // the position variable has attribute position 0
layout (location = 1) in vec2 a_TextureCoordinate;
layout (location = 2) in int a_TextureIndex;
layout (location = 3) in vec4 a_Color;

out vec2  v_TextureCoordinate;
flat out int v_TextureIndex;
out vec4  v_Color;

uniform mat4 u_ViewProjectionMatrix;

void main()
{
    gl_Position = u_ViewProjectionMatrix * a_Position;

    v_TextureCoordinate = a_TextureCoordinate;
    v_TextureIndex = a_TextureIndex;
    v_Color = a_Color;
};
