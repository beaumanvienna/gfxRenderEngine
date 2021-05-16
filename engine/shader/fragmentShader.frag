#version 330 core

// output to frame buffer
layout(location = 0) out vec4 color;

//input from vertex shader / vertex array buffer
in vec2 v_TextureCoordinate;
in float v_TextureIndex;

//uniforms
uniform vec4 u_Color;
uniform sampler2D u_Textures[8];
//uniform sampler2D u_Texture;

void main()
{
    int textureIndex = int(v_TextureIndex);
    
    switch (textureIndex) 
    {
        case 0:
            color = texture(u_Textures[0], v_TextureCoordinate);
            break;
        case 1:
            color = texture(u_Textures[1], v_TextureCoordinate);
            break;
        case 2:
            color = texture(u_Textures[2], v_TextureCoordinate);
            break;
        case 3:
            color = texture(u_Textures[3], v_TextureCoordinate);
            break;
        case 4:
            color = texture(u_Textures[4], v_TextureCoordinate);
            break;
        case 5:
            color = texture(u_Textures[5], v_TextureCoordinate);
            break;
        case 6:
            color = texture(u_Textures[6], v_TextureCoordinate);
            break;
        case 7:
            color = texture(u_Textures[7], v_TextureCoordinate);
            break;
    }
}; 
