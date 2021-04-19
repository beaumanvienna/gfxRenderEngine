/* Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/gfxRenderEngine

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <fstream>

#include "shader.h"
#include "OpenGL/GL.h"

int gShaderStatus = SHADER_STATUS_UNDEFINED;

bool LoadShaderFromFile(std::string& shader, const std::string filename)
{   
    bool ok = false;
    std::string line;
    std::ifstream shaderFile(filename);
    if (shaderFile.is_open())
    {
        shader = "";
        while ( getline(shaderFile,line) )
        {
            shader += line;
            shader += '\n';
        }
        if (shader.size())
        {
            // all good
            ok = true;
        }
        else
        {
            gShaderStatus = SHADER_ERROR_EMPTY_FILE;
        }
        shaderFile.close();
    }
    else
    {
        gShaderStatus = SHADER_ERROR_COULD_NOT_LOAD_FILE;
    }
    return ok;
}

#define SIZE_OF_INFOLOG 512
static uint CompileShader(const int type, const std::string& shader)
{
    uint shaderID = glCreateShader(type);
    const char* shaderCode = shader.c_str();
    int success;
    char infoLog[SIZE_OF_INFOLOG];

    gShaderStatus = SHADER_OK;
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    // print compile errors if any
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, SIZE_OF_INFOLOG, NULL, infoLog);
        std::cout << "shader compilation failed in " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << infoLog << std::endl;
        gShaderStatus = SHADER_ERROR_COMPILE_FAILED;
        return INVALID_ID;
    };
    
    return shaderID;
}

int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    int shaderID = SHADER_ID_INVALID;
    int success;
    uint vertexShaderID, fragmentShaderID;
    char infoLog[SIZE_OF_INFOLOG];
    
    // vertex shader
    vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShader);
    if (gShaderStatus == SHADER_OK) 
    {
        // fragment shader
        fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
        if (gShaderStatus == SHADER_OK) 
        {
            int shaderID = glCreateProgram();
            glAttachShader(shaderID, vertexShaderID);
            glAttachShader(shaderID, fragmentShaderID);
            glLinkProgram(shaderID);
            
            // print linking errors if any
            glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
            if(success)
            {
                glValidateProgram(shaderID);
                glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &success);
                if (success)
                {
                    gShaderStatus = SHADER_OK;
                    glUseProgram(shaderID);
                    std::cout << "Shader creation successful" << std::endl;
                    return shaderID;
                }
                else
                {
                    gShaderStatus = SHADER_ERROR_VALIDATION_FAILED;
                    glGetProgramInfoLog(shaderID, SIZE_OF_INFOLOG, NULL, infoLog);
                    std::cout << "Program validation failed" << infoLog << std::endl;
                }
            }
            else
            {
                gShaderStatus = SHADER_ERROR_CREATION_FAILED;
                glGetProgramInfoLog(shaderID, SIZE_OF_INFOLOG, NULL, infoLog);
                std::cout << "Program linking failed" << infoLog << std::endl;
            }
        }
        glDeleteShader(fragmentShaderID);
    }
    glDeleteShader(vertexShaderID);
    
    return INVALID_ID;
}

