#version 330 core

// output to frame buffer
layout(location = 0) out vec4 color;

//input from vertex shader / vertex array buffer
in vec2 v_TextureCoordinate;

//uniforms
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TextureCoordinate);
    color = texColor;
}; 
