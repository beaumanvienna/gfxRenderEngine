#version 330 core

// output to frame buffer
layout(location = 0) out vec4 color;

//input from vertex shader / vertex array buffer
in vec2 v_TextureCoordinate;
flat in int v_TextureIndex;
in vec4 v_Color;

//uniforms
uniform sampler2D u_Textures[16];

void main()
{
    vec4 errorCodeBlue = vec4(0.0,0.0,1.0,1.0);
    
    switch (v_TextureIndex)
    {
        case 0:
            color = texture(u_Textures[0], v_TextureCoordinate) * v_Color;
            break;
        case 1:
            color = texture(u_Textures[1], v_TextureCoordinate) * v_Color;
            break;
        case 2:
            color = texture(u_Textures[2], v_TextureCoordinate) * v_Color;
            break;
        case 3:
            color = texture(u_Textures[3], v_TextureCoordinate) * v_Color;
            break;
        case 4:
            color = texture(u_Textures[4], v_TextureCoordinate) * v_Color;
            break;
        case 5:
            color = texture(u_Textures[5], v_TextureCoordinate) * v_Color;
            break;
        case 6:
            color = texture(u_Textures[6], v_TextureCoordinate) * v_Color;
            break;
        case 7:
            color = texture(u_Textures[7], v_TextureCoordinate) * v_Color;
            break;
        case 8:
            color = texture(u_Textures[8], v_TextureCoordinate) * v_Color;
            break;
        case 9:
            color = texture(u_Textures[9], v_TextureCoordinate) * v_Color;
            break;
        case 10:
            color = texture(u_Textures[10], v_TextureCoordinate) * v_Color;
            break;
        case 11:
            color = texture(u_Textures[11], v_TextureCoordinate) * v_Color;
            break;
        case 12:
            color = texture(u_Textures[12], v_TextureCoordinate) * v_Color;
            break;
        case 13:
            color = texture(u_Textures[13], v_TextureCoordinate) * v_Color;
            break;
        case 14:
            color = texture(u_Textures[14], v_TextureCoordinate) * v_Color;
            break;
        case 15:
            color = texture(u_Textures[15], v_TextureCoordinate) * v_Color;
            break;
        default:
            //color = vec4(v_TextureCoordinate.x,v_TextureCoordinate.y,0.2,1.0);
            color = errorCodeBlue;
            break;
    }
}; 
