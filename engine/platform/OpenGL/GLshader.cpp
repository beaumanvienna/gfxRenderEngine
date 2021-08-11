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

#include "GLshader.h"
#include "GL.h"
#include "log.h"
#include "resources.h"

#define SIZE_OF_INFOLOG 512

GLShaderProgram::GLShaderProgram()
    : m_RendererID(0), m_ShadersAreLoaded(true), m_ShaderStatus(SHADER_STATUS_UNDEFINED)
{
}

GLShaderProgram::~GLShaderProgram()
{
    GLCall(glDeleteProgram(m_RendererID));
}

int GLShaderProgram::AddShader(const ShaderProgramTypes type, const std::string& shaderFileName)
{
    int shaderProgramType;
    switch(type)
    {
        case VERTEX_SHADER:
            shaderProgramType = GL_VERTEX_SHADER;
            break;
        case FRAGMENT_SHADER:
            shaderProgramType = GL_FRAGMENT_SHADER;
            break;
    }
    GLShader shader(shaderProgramType,shaderFileName);
    return AddShader(shader);
}

int GLShaderProgram::AddShader(const ShaderProgramTypes type, const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */)
{
    int shaderProgramType;
    switch(type)
    {
        case VERTEX_SHADER:
            shaderProgramType = GL_VERTEX_SHADER;
            break;
        case FRAGMENT_SHADER:
            shaderProgramType = GL_FRAGMENT_SHADER;
            break;
    }
    GLShader shader(shaderProgramType, path, resourceID, resourceClass);
    return AddShader(shader);
}

int GLShaderProgram::AddShader(GLShader& shader)
{
    shader.Compile();

    bool shaderLoaded = shader.IsOK();
    m_ShadersAreLoaded &= shaderLoaded;

    if (!shaderLoaded)
    {
        LOG_CORE_CRITICAL("Couldn't load shader");
        return INVALID_ID;
    }
    else
    {
        m_Shaders.push_back(shader);
    }

    return shader.ID();
}
int GLShaderProgram::Build()
{
    m_RendererID = INVALID_ID;
    if (m_ShadersAreLoaded)
    {
        char infoLog[SIZE_OF_INFOLOG];
        m_RendererID = glCreateProgram();

        for (auto shader : m_Shaders)
        {
            glAttachShader(m_RendererID, shader.ID());
        }

        glLinkProgram(m_RendererID);
        
        // print linking errors if any
        int success;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
        if(success)
        {
            glValidateProgram(m_RendererID);
            glGetProgramiv(m_RendererID, GL_VALIDATE_STATUS, &success);
            if (success)
            {
                m_ShaderStatus = SHADER_OK;
                Bind();
                LOG_CORE_INFO("Shader creation successful");
            }
            else
            {
                m_ShaderStatus = SHADER_ERROR_VALIDATION_FAILED;
                glGetProgramInfoLog(m_RendererID, SIZE_OF_INFOLOG, NULL, infoLog);
                std::cout << "Program validation failed" << infoLog << std::endl;
            }
        }
        else
        {
            m_ShaderStatus = SHADER_ERROR_CREATION_FAILED;
            glGetProgramInfoLog(m_RendererID, SIZE_OF_INFOLOG, NULL, infoLog);
            std::cout << "Program linking failed" << infoLog << std::endl;
        }
        
        if (m_ShaderStatus != SHADER_OK)
        {
            std::cout << "Shader creation failed" << std::endl;
            m_RendererID = INVALID_ID;
        }
        for (auto shader : m_Shaders)
        {
            shader.Unbind();
        }
    }
    
    return m_RendererID;
}

void GLShaderProgram::Bind() const
{
    glUseProgram(m_RendererID);
}
    
void GLShaderProgram::Unbind() const
{
    glUseProgram(INVALID_ID);
}

GLShader::GLShader(const int type, const std::string fileName) 
    : m_Type(type), m_FileName(fileName), m_RendererID(0)
{
    LoadFromFile();
}

GLShader::GLShader(const int type, const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */)
    : m_Type(type), m_RendererID(0)
{
    LoadFromMemory(path, resourceID, resourceClass);
}

GLShader::~GLShader()
{
}

void GLShader::Bind()
{
    m_RendererID = glCreateShader(m_Type);
}
    
void GLShader::Unbind() const
{
    glDeleteShader(m_RendererID);
}

int GLShaderProgram::GetUniformLocation(const std::string& name)
{
    int uniformLocation;
    //check cache
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end())
    {
        // cache hit
        uniformLocation = it->second;
    }
    else
    {
        //cache miss
        GLCall(uniformLocation = glGetUniformLocation(m_RendererID, name.c_str()));
        ASSERT(uniformLocation != -1);
        if (uniformLocation == -1)
        {
            std::cout << " (uniform '" << name << ", not found)"<< std::endl;
        }
        uniformLocationCache.insert( std::pair<std::string,int>(name,uniformLocation));
    }
    return uniformLocation;
}

void GLShaderProgram::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int uniformLocation = GetUniformLocation(name);
    GLCall(glUniform4f(uniformLocation, v0, v1, v2, v3));
}

void GLShaderProgram::SetUniform1i(const std::string& name, int i0)
{
    int uniformLocation = GetUniformLocation(name);
    GLCall(glUniform1i(uniformLocation, i0));
} 

void GLShaderProgram::SetUniform1iv(const std::string& name, int count, int* i0)
{
    int uniformLocation = GetUniformLocation(name);
    GLCall(glUniform1iv(uniformLocation, count, i0));
} 

void GLShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& modelViewProjection)
{
    int uniformLocation = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &modelViewProjection[0][0]));
}

bool GLShader::LoadFromFile()
{
    m_ShaderIsLoaded = false;
    std::string line;
    std::ifstream shaderFile(m_FileName);
    if (shaderFile.is_open())
    {
        m_ShaderSourceCode = "";
        while ( getline(shaderFile,line) )
        {
            m_ShaderSourceCode += line;
            m_ShaderSourceCode += '\n';
        }
        if (m_ShaderSourceCode.size())
        {
            // all good
            m_ShaderIsLoaded = true;
        }
        else
        {
            m_ShaderStatus = SHADER_ERROR_EMPTY_FILE;
        }
        shaderFile.close();
    }
    else
    {
        m_ShaderStatus = SHADER_ERROR_COULD_NOT_LOAD_FILE;
    }
    return m_ShaderIsLoaded;
}

bool GLShader::LoadFromMemory(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */)
{

    std::string_view shaderCode;
    m_ShaderIsLoaded = ResourceSystem::GetResourceString(shaderCode, path, resourceID, resourceClass);

    if (m_ShaderIsLoaded)
    {
        m_ShaderSourceCode = shaderCode;
    }
    else
    {
        m_ShaderStatus = SHADER_ERROR_COULD_NOT_LOAD_FILE;
    }
    return m_ShaderIsLoaded;
}

bool GLShader::Compile()
{
    m_ShaderStatus = SHADER_STATUS_UNDEFINED;
    if (m_ShaderIsLoaded)
    {
        Bind();
        const char* shaderCode = m_ShaderSourceCode.c_str();
        int success;
        char infoLog[SIZE_OF_INFOLOG];

        glShaderSource(m_RendererID, 1, &shaderCode, NULL);
        glCompileShader(m_RendererID);
        // print compile errors if any
        glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(m_RendererID, SIZE_OF_INFOLOG, NULL, infoLog);
            std::cout << "shader compilation failed in " << (m_Type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
            std::cout << infoLog << std::endl;
            m_ShaderStatus = SHADER_ERROR_COMPILE_FAILED;
        }
        else
        {
            m_ShaderStatus = SHADER_OK;
        }
    }
    
    return (m_ShaderStatus == SHADER_OK);

}
