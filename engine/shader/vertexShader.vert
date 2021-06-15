#version 330 core

layout (location = 0) in vec4 a_Position; // the position variable has attribute position 0
layout (location = 1) in vec2 a_TextureCoordinate;
layout (location = 2) in float a_TextureIndex;

out vec2  v_TextureCoordinate;
out float v_TextureIndex;

//mat4 model_view_projection;
//uniform mat4 u_ViewProjectionMatrix;
//uniform mat4 u_NormalizedPosition;

void main()
{
    //gl_Position = u_ViewProjectionMatrix * a_Position;
    //model_view_projection = u_ViewProjectionMatrix * modelMatrix;
    gl_Position = a_Position;
    v_TextureCoordinate = a_TextureCoordinate;
    v_TextureIndex = a_TextureIndex;
};
